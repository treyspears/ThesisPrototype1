// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "MyGameInstance.h"
#include "AdvertisingActorSensingComponent.h"

DECLARE_CYCLE_STAT( TEXT( "Sensing" ), STAT_AI_Sensing, STATGROUP_AI );

// Sets default values for this component's properties
UAdvertisingActorSensingComponent::UAdvertisingActorSensingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = false;

	SightRadius = 5000.0f;
	PeripheralVisionAngle = 90.f;
	PeripheralVisionCosine = FMath::Cos( FMath::DegreesToRadians( PeripheralVisionAngle ) );

	SensingInterval = 0.5f;

	bSeeActors = true;
	bEnableSensingUpdates = true;
}

// Called when the game starts
void UAdvertisingActorSensingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetPeripheralVisionAngle( PeripheralVisionAngle );

	if ( bEnableSensingUpdates )
	{
		bEnableSensingUpdates = false; // force an update
		SetSensingUpdatesEnabled( true );
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::SetPeripheralVisionAngle( const float NewPeripheralVisionAngle )
{
	PeripheralVisionAngle = NewPeripheralVisionAngle;
	PeripheralVisionCosine = FMath::Cos( FMath::DegreesToRadians( PeripheralVisionAngle ) );
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::SetSensingUpdatesEnabled( const bool bEnabled )
{
	if ( bEnableSensingUpdates != bEnabled )
	{
		bEnableSensingUpdates = bEnabled;

		if ( bEnabled && SensingInterval > 0.f )
		{
			// Stagger initial updates so all sensors do not update at the same time (to avoid hitches).
			const float InitialDelay = ( SensingInterval * FMath::SRand() ) + KINDA_SMALL_NUMBER;
			SetTimer( InitialDelay );
		}
		else
		{
			SetTimer( 0.f );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::SetTimer( const float TimeInterval )
{
	// Only necessary to update if we are the server
	AActor* const Owner = GetOwner();
	if ( IsValid( Owner ) && GEngine->GetNetMode( GetWorld() ) < NM_Client )
	{
		Owner->GetWorldTimerManager().SetTimer( TimerHandle_OnTimer, this, &UAdvertisingActorSensingComponent::OnTimer, TimeInterval, false );
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::SetSensingInterval( const float NewSensingInterval )
{
	if ( SensingInterval != NewSensingInterval )
	{
		SensingInterval = NewSensingInterval;

		AActor* const Owner = GetOwner();
		if ( IsValid( Owner ) )
		{
			if ( SensingInterval <= 0.f )
			{
				SetTimer( 0.f );
			}
			else if ( bEnableSensingUpdates )
			{
				float CurrentElapsed = Owner->GetWorldTimerManager().GetTimerElapsed( TimerHandle_OnTimer );
				CurrentElapsed = FMath::Max( 0.f, CurrentElapsed );

				if ( CurrentElapsed < SensingInterval )
				{
					// Extend lifetime by remaining time.
					SetTimer( SensingInterval - CurrentElapsed );
				}
				else if ( CurrentElapsed > SensingInterval )
				{
					// Basically fire next update, because time has already expired.
					// Don't want to fire immediately in case an update tries to change the interval, looping endlessly.
					SetTimer( KINDA_SMALL_NUMBER );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::OnTimer()
{
	SCOPE_CYCLE_COUNTER( STAT_AI_Sensing );

	AActor* const Owner = GetOwner();
	if ( !IsValid( Owner ) || !IsValid( Owner->GetWorld() ) )
	{
		return;
	}

	if ( CanSenseAnything() )
	{
		UpdateAISensing();
	}

	SetTimer( SensingInterval );
};

//-----------------------------------------------------------------------------------------------
AActor* UAdvertisingActorSensingComponent::GetSensorActor() const
{
	AActor* SensorActor = GetOwner();
	AController* Controller = Cast<AController>( SensorActor );
	if ( IsValid( Controller ) )
	{
		// In case the owner is a controller, use the controlled pawn as the sensing location.
		SensorActor = Controller->GetPawn();
	}

	if ( !IsValid( SensorActor ) )
	{
		return NULL;
	}

	return SensorActor;
}

//-----------------------------------------------------------------------------------------------
AController* UAdvertisingActorSensingComponent::GetSensorController() const
{
	AActor* SensorActor = GetOwner();
	AController* Controller = Cast<AController>( SensorActor );

	if ( IsValid( Controller ) )
	{
		return Controller;
	}
	else
	{
		APawn* Pawn = Cast<APawn>( SensorActor );
		if ( IsValid( Pawn ) && IsValid( Pawn->Controller ) )
		{
			return Pawn->Controller;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------
bool UAdvertisingActorSensingComponent::IsSensorActor( const AActor* Actor ) const
{
	return ( Actor == GetSensorActor() );
}

//-----------------------------------------------------------------------------------------------
bool UAdvertisingActorSensingComponent::HasLineOfSightTo( const AActor* Other ) const
{
	AController* SensorController = GetSensorController();
	if ( SensorController == NULL )
	{
		return false;
	}

	return SensorController->LineOfSightTo( Other, FVector::ZeroVector, true );
}

//-----------------------------------------------------------------------------------------------
bool UAdvertisingActorSensingComponent::CanSenseAnything() const
{
	return ( ( bSeeActors && OnSeeActor.IsBound() ) );
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::UpdateAISensing()
{
	AActor* const Owner = GetOwner();
	check( IsValid( Owner ) );
	UWorld* const World = Owner->GetWorld();
	check( IsValid( World ) );
	UMyGameInstance* const GameInstance = Cast< UMyGameInstance >( World->GetGameInstance() );
	check( IsValid( GameInstance ) );


	for ( FConstActorsWithAdvertismentIterator Iterator = GameInstance->GetActorsWithAdvertisementsIterator(); Iterator; ++Iterator )
	{
		AActor* Actor = *Iterator;
		if ( IsValid( Actor ) && !IsSensorActor( Actor ) )
		{
			SenseActor( *Actor );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::SenseActor( AActor& Actor )
{
	// Visibility checks
	bool bHasSeenActor = false;
	bool bHasFailedLineOfSightCheck = false;
	if ( bSeeActors && ShouldCheckVisibilityOf( &Actor ) )
	{
		if ( CouldSeeActor( &Actor, true ) )
		{
			if ( HasLineOfSightTo( &Actor ) )
			{
				BroadcastOnSeeActor( Actor );
				bHasSeenActor = true;
			}
			else
			{
				bHasFailedLineOfSightCheck = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisingActorSensingComponent::BroadcastOnSeeActor( AActor& Actor )
{
	OnSeeActor.Broadcast( &Actor );
}

//-----------------------------------------------------------------------------------------------
bool UAdvertisingActorSensingComponent::CouldSeeActor( const AActor *Other, bool bMaySkipChecks ) const
{
	if ( !Other )
	{
		return false;
	}

	const AActor* Owner = GetOwner();
	if ( !Owner )
	{
		return false;
	}

	FVector const OtherLoc = Other->GetActorLocation();
	FVector const SensorLoc = GetSensorLocation();
	FVector const SelfToOther = OtherLoc - SensorLoc;

	// check max sight distance
	float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
	if ( SelfToOtherDistSquared > FMath::Square( SightRadius ) )
	{
		return false;
	}

	// may skip if more than some fraction of maxdist away (longer time to acquire)
	if ( bMaySkipChecks && ( FMath::Square( FMath::FRand() ) * SelfToOtherDistSquared > FMath::Square( 0.4f * SightRadius ) ) )
	{
		return false;
	}

	// 	UE_LOG(LogPath, Warning, TEXT("DistanceToOtherSquared = %f, SightRadiusSquared: %f"), SelfToOtherDistSquared, FMath::Square(SightRadius));

	// check field of view
	FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
	FVector const MyFacingDir = Owner->GetActorRotation().Vector();

	// 	UE_LOG(LogPath, Warning, TEXT("DotProductFacing: %f, PeripheralVisionCosine: %f"), SelfToOtherDir | MyFacingDir, PeripheralVisionCosine);

	return ( ( SelfToOtherDir | MyFacingDir ) >= PeripheralVisionCosine );
}

//-----------------------------------------------------------------------------------------------
FVector UAdvertisingActorSensingComponent::GetSensorLocation() const
{
	FVector SensorLocation( FVector::ZeroVector );
	const AActor* SensorActor = GetSensorActor();

	if ( SensorActor != NULL )
	{
		SensorLocation = SensorActor->GetActorLocation();
		const APawn* SensorPawn = Cast<const APawn>( SensorActor );
		if ( SensorPawn != NULL )
		{
			SensorLocation.Z += SensorPawn->BaseEyeHeight;
		}
	}

	return SensorLocation;
}

//-----------------------------------------------------------------------------------------------
FRotator UAdvertisingActorSensingComponent::GetSensorRotation() const
{
	FRotator SensorRotation( FRotator::ZeroRotator );
	const AActor* SensorActor = GetSensorActor();

	if ( SensorActor != NULL )
	{
		SensorRotation = SensorActor->GetActorRotation();
	}

	return SensorRotation;
}

//-----------------------------------------------------------------------------------------------
bool UAdvertisingActorSensingComponent::ShouldCheckVisibilityOf( AActor *Actor ) const
{
	if ( !bSeeActors )
	{
		return false;
	}

	if ( Actor->bHidden )
	{
		return false;
	}

	return true;
}