// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "GameSpectatorPawn.h"
#include "Components/ChildActorComponent.h"

/*
	AGameSpectatorPawn Implementation
*/
//-----------------------------------------------------------------------------------------------
AGameSpectatorPawn::AGameSpectatorPawn( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, CurrentOrbitTarget( nullptr )
	, DistanceToOrbitTarget( FVector::ZeroVector )
	, InitialTopDownCameraWorldZLocation( 1024.f )
	, TopDownCameraRotation( -90.f, 90.f, 0.f )
{
	FirstPersonCameraComponent = CreateDefaultSubobject< UChildActorComponent >( TEXT( "First Person Camera" ) );
	FirstPersonCameraComponent->AttachParent = RootComponent;
	FirstPersonCameraComponent->ChildActorClass = ACameraActor::StaticClass();

	TopDownCameraComponent = CreateDefaultSubobject< UChildActorComponent >( TEXT( "Top Down Camera" ) );
	TopDownCameraComponent->AttachParent = RootComponent;
	TopDownCameraComponent->ChildActorClass = ACameraActor::StaticClass();

	static FName CollisionProfileName( TEXT( "NoCollision" ) );
	GetCollisionComponent()->SetCollisionProfileName( CollisionProfileName );

	GameSpectatorPawn_State::FirstPersonFlying::EnterState( this );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::SetupPlayerInputComponent( class UInputComponent* InputComponent )
{
	Super::SetupPlayerInputComponent( InputComponent );

	InputComponent->BindAxis( "TurnRate", this, &AGameSpectatorPawn::TurnAtRate );
	InputComponent->BindAxis( "LookUpRate", this, &AGameSpectatorPawn::LookUpAtRate );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	InitializeCameras();
	
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	CurrentState->Tick( this, DeltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::InitializeCameras()
{
	ACameraActor* ChildActorAsCameraActor = nullptr;

	ChildActorAsCameraActor = Cast< ACameraActor>( FirstPersonCameraComponent->ChildActor );

	if ( ChildActorAsCameraActor )
	{
		UCameraComponent* CameraComponent;
		CameraComponent = ChildActorAsCameraActor->GetCameraComponent();

		if ( CameraComponent )
		{
			CameraComponent->bUsePawnControlRotation = true;
			CameraComponent->bConstrainAspectRatio = false;
		}
	}

	ChildActorAsCameraActor = Cast< ACameraActor>( TopDownCameraComponent->ChildActor );

	if ( ChildActorAsCameraActor )
	{
		ChildActorAsCameraActor->SetActorRotation( TopDownCameraRotation );

		UCameraComponent* CameraComponent;
		CameraComponent = ChildActorAsCameraActor->GetCameraComponent();

		if ( CameraComponent )
		{
			CameraComponent->bUsePawnControlRotation = false;
			CameraComponent->bConstrainAspectRatio = false;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::SetViewPitchMinMax( float NewMinAngle, float NewMaxAngle )
{
	APlayerController* PlayerController = Cast< APlayerController >( Controller );

	if ( PlayerController )
	{
		if ( PlayerController->PlayerCameraManager )
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = NewMinAngle;
			PlayerController->PlayerCameraManager->ViewPitchMax = NewMaxAngle;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::MoveForward( float Val )
{
	CurrentState->MoveForward( this,  Val );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::MoveRight( float Val )
{
	CurrentState->MoveRight( this, Val );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::MoveUp_World( float Val )
{
	CurrentState->MoveUp_World( this, Val );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::TurnAtRate( float Rate )
{
	CurrentState->TurnAtRate( this, Rate );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::LookUpAtRate( float Rate )
{
	CurrentState->LookUpAtRate( this, Rate );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::OrbitSelectedActor( const FHitResult& CurrentMouseClickResult )
{
	if( CurrentMouseClickResult.Actor.IsValid() )
	{
		CurrentOrbitTarget = CurrentMouseClickResult.Actor;
	}

	if( CurrentOrbitTarget.IsValid() )
	{
		CurrentState->OrbitSelectedActor( this );
	}
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::OnEnterFirstPersonView()
{
	if( CurrentOrbitTarget.IsValid() )
	{
		//if( CurrentState == GameSpectatorPawn_State::FirstPersonOrbit::GetState() )
		//{
			CurrentOrbitTarget = nullptr;
			GameSpectatorPawn_State::FirstPersonFlying::EnterState( this );
		//}
		//else
		//{
		//	GameSpectatorPawn_State::FirstPersonOrbit::EnterState( this );
		//}	
	}
	else
	{
		GameSpectatorPawn_State::FirstPersonFlying::EnterState( this );
	}
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::OnEnterTopDownView()
{
	MovePawnToTopDownCameraZPosition();

	if ( CurrentOrbitTarget.IsValid() )
	{
		//if ( CurrentState == GameSpectatorPawn_State::TopDownAttached::GetState() )
		//{
			CurrentOrbitTarget = nullptr;
			GameSpectatorPawn_State::TopDownUnAttached::EnterState( this );
		//}
		//else
		//{
		//	GameSpectatorPawn_State::TopDownAttached::EnterState( this );
		//}
	}
	else
	{
		GameSpectatorPawn_State::TopDownUnAttached::EnterState( this );
	}
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::MovePawnToTopDownCameraZPosition()
{
	FVector CameraLocation = GetActorLocation();
	CameraLocation.Z = InitialTopDownCameraWorldZLocation;

	SetActorLocation( CameraLocation );
}

//-----------------------------------------------------------------------------------------------
void AGameSpectatorPawn::OnNewMouseHitResult( const FHitResult& NewMouseHitResult )
{
	//This will need to be changed to reflect lock system

}


#pragma region AGameSpectatorPawn States

/*
	FirstPersonFlying Implementation
*/

GameSpectatorPawn_State::FirstPersonFlying GameSpectatorPawn_State::FirstPersonFlying::Singleton;

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::EnterState( AGameSpectatorPawn* Pawn )
{
	if ( Pawn->GetCurrentState() != &Singleton )
	{
		const float FIRST_PERSON_MIN_MAX_PITCH_ANGLE = 90.f;
		Pawn->SetViewPitchMinMax( -FIRST_PERSON_MIN_MAX_PITCH_ANGLE, FIRST_PERSON_MIN_MAX_PITCH_ANGLE );

		Pawn->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
const GameSpectatorPawn_State::FirstPersonFlying* GameSpectatorPawn_State::FirstPersonFlying::GetState()
{
	return &Singleton;
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds )
{

}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::MoveForward( AGameSpectatorPawn* Pawn, float Val )
{
	Pawn->ADefaultPawn::MoveForward( Val );
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::MoveRight( AGameSpectatorPawn* Pawn, float Val )
{
	Pawn->ADefaultPawn::MoveRight( Val );
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::MoveUp_World( AGameSpectatorPawn* Pawn, float Val )
{
	Pawn->ADefaultPawn::MoveUp_World( Val );
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::TurnAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
	Pawn->ADefaultPawn::TurnAtRate( Rate );
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
	Pawn->ADefaultPawn::LookUpAtRate( Rate );
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonFlying::OrbitSelectedActor( AGameSpectatorPawn* Pawn )
{
	GameSpectatorPawn_State::FirstPersonOrbit::EnterState( Pawn );
}

/*
FirstPersonOrbit Implementation
*/

GameSpectatorPawn_State::FirstPersonOrbit GameSpectatorPawn_State::FirstPersonOrbit::Singleton;
FVector GameSpectatorPawn_State::FirstPersonOrbit::PreviousAttachedTargetPosition = FVector::ZeroVector;
FRotator GameSpectatorPawn_State::FirstPersonOrbit::PreviousViewRotation = FRotator::ZeroRotator;
float GameSpectatorPawn_State::FirstPersonOrbit::CurrentMinDistanceFromTarget = 0.f;
GameSpectatorPawn_State::FirstPersonOrbit::FActorBounds GameSpectatorPawn_State::FirstPersonOrbit::CurrentTargetBounds;

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::EnterState( AGameSpectatorPawn* Pawn )
{
	PreviousAttachedTargetPosition = Pawn->CurrentOrbitTarget->GetActorLocation();
	PreviousViewRotation = Pawn->GetViewRotation();

	Pawn->CurrentOrbitTarget->GetActorBounds( true, CurrentTargetBounds.Origin, CurrentTargetBounds.BoxExtent );
	CurrentTargetBounds.bIsDirty = false;
	
	float MaxDimension = FMath::Max3< float >( CurrentTargetBounds.BoxExtent.X, CurrentTargetBounds.BoxExtent.Y, CurrentTargetBounds.BoxExtent.Z );

	const float MAX_DISTANCE_SCALAR = 2.5f;

	CurrentMinDistanceFromTarget = MaxDimension * MAX_DISTANCE_SCALAR;

	FVector PawnViewDirection = PreviousViewRotation.Vector();
	
	FVector NewPawnLocation = CurrentTargetBounds.Origin - ( PawnViewDirection * CurrentMinDistanceFromTarget );
	Pawn->SetActorLocation( NewPawnLocation );

	if ( Pawn->GetCurrentState() != &Singleton )
	{
		const float FIRST_PERSON_MIN_MAX_PITCH_ANGLE = 359.f;
		Pawn->SetViewPitchMinMax( -FIRST_PERSON_MIN_MAX_PITCH_ANGLE, FIRST_PERSON_MIN_MAX_PITCH_ANGLE );

		Pawn->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
const GameSpectatorPawn_State::FirstPersonOrbit* GameSpectatorPawn_State::FirstPersonOrbit::GetState()
{
	return &Singleton;
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds )
{
	if ( Pawn->CurrentOrbitTarget.IsValid() )
	{
		FVector AttachedActorCurrentPosition = Pawn->CurrentOrbitTarget->GetActorLocation();

		FVector DeltaPosition = AttachedActorCurrentPosition - PreviousAttachedTargetPosition;

		if( !DeltaPosition.IsNearlyZero() )
		{
			CurrentTargetBounds.bIsDirty = true;
		}

		Pawn->SetActorLocation( Pawn->GetActorLocation() + DeltaPosition );

		PreviousAttachedTargetPosition = AttachedActorCurrentPosition;

		FRotator CurrentViewRotation = Pawn->GetViewRotation();

		FRotator DeltaRotation = CurrentViewRotation - PreviousViewRotation;

		if( !DeltaRotation.IsNearlyZero() )
		{
			if( CurrentTargetBounds.bIsDirty )
			{
				Pawn->CurrentOrbitTarget->GetActorBounds( true, CurrentTargetBounds.Origin, CurrentTargetBounds.BoxExtent );
				CurrentTargetBounds.bIsDirty = false;
			}

			float DistanceFromTarget = FVector::Dist( CurrentTargetBounds.Origin, Pawn->GetActorLocation() );

			FVector NewPawnLocation = CurrentTargetBounds.Origin - ( CurrentViewRotation.Vector() * DistanceFromTarget );
			Pawn->SetActorLocation( NewPawnLocation );
		}

		PreviousViewRotation = CurrentViewRotation;
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::MoveForward( AGameSpectatorPawn* Pawn, float Val )
{
	if ( CurrentTargetBounds.bIsDirty )
	{
		Pawn->CurrentOrbitTarget->GetActorBounds( true, CurrentTargetBounds.Origin, CurrentTargetBounds.BoxExtent );
		CurrentTargetBounds.bIsDirty = false;
	}

	Pawn->ADefaultPawn::MoveForward( Val );

	float DistanceSquaredFromTarget = FVector::DistSquared( CurrentTargetBounds.Origin, Pawn->GetActorLocation() );

	if( DistanceSquaredFromTarget < CurrentMinDistanceFromTarget * CurrentMinDistanceFromTarget )
	{
		FVector NewPawnLocation = CurrentTargetBounds.Origin - ( PreviousViewRotation.Vector() * CurrentMinDistanceFromTarget );
		Pawn->SetActorLocation( NewPawnLocation );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::MoveRight( AGameSpectatorPawn* Pawn, float Val )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::MoveUp_World( AGameSpectatorPawn* Pawn, float Val )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::TurnAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::FirstPersonOrbit::OrbitSelectedActor( AGameSpectatorPawn* Pawn )
{
	EnterState( Pawn );
}

/*
TopDownUnAttached Implementation
*/

GameSpectatorPawn_State::TopDownUnAttached GameSpectatorPawn_State::TopDownUnAttached::Singleton;

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::EnterState( AGameSpectatorPawn* Pawn )
{
	if ( Pawn->GetCurrentState() != &Singleton )
	{
		Pawn->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
const GameSpectatorPawn_State::TopDownUnAttached* GameSpectatorPawn_State::TopDownUnAttached::GetState()
{
	return &Singleton;
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds )
{

}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::MoveForward( AGameSpectatorPawn* Pawn, float Val )
{
	const FVector NorthVector( 0.f, 1.f, 0.f );

	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( NorthVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::MoveRight( AGameSpectatorPawn* Pawn, float Val )
{
	const FVector WestVector( -1.f, 0.f, 0.f );

	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( WestVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::MoveUp_World( AGameSpectatorPawn* Pawn, float Val )
{
	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( FVector::UpVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::TurnAtRate( AGameSpectatorPawn* Pawn, float Rate )
{

}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate )
{

}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownUnAttached::OrbitSelectedActor( AGameSpectatorPawn* Pawn )
{
	GameSpectatorPawn_State::TopDownAttached::EnterState( Pawn );
}

/*
TopDownAttached Implementation
*/

GameSpectatorPawn_State::TopDownAttached GameSpectatorPawn_State::TopDownAttached::Singleton;
FVector GameSpectatorPawn_State::TopDownAttached::PreviousAttachedTargetPosition;

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::EnterState( AGameSpectatorPawn* Pawn )
{
	FVector CurrentOrbitTargetLocation = Pawn->CurrentOrbitTarget->GetActorLocation();

	FVector NewLocation = CurrentOrbitTargetLocation;
	NewLocation.Z = Pawn->InitialTopDownCameraWorldZLocation;

	Pawn->SetActorLocation( NewLocation );

	PreviousAttachedTargetPosition = CurrentOrbitTargetLocation;

	if ( Pawn->GetCurrentState() != &Singleton )
	{
		Pawn->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
const GameSpectatorPawn_State::TopDownAttached* GameSpectatorPawn_State::TopDownAttached::GetState()
{
	return &Singleton;
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds )
{
	if( Pawn->CurrentOrbitTarget.IsValid() )
	{
		FVector AttachedActorCurrentPosition = Pawn->CurrentOrbitTarget->GetActorLocation();

		FVector DeltaPosition = AttachedActorCurrentPosition - PreviousAttachedTargetPosition;

		Pawn->SetActorLocation( Pawn->GetActorLocation() + DeltaPosition );

		PreviousAttachedTargetPosition = AttachedActorCurrentPosition;
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::MoveForward( AGameSpectatorPawn* Pawn, float Val )
{
	const FVector NorthVector( 0.f, 1.f, 0.f );

	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( NorthVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::MoveRight( AGameSpectatorPawn* Pawn, float Val )
{
	const FVector WestVector( -1.f, 0.f, 0.f );

	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( WestVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::MoveUp_World( AGameSpectatorPawn* Pawn, float Val )
{
	if ( Val != 0.f )
	{
		Pawn->AddMovementInput( FVector::UpVector, Val );
	}
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::TurnAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::TopDownAttached::OrbitSelectedActor( AGameSpectatorPawn* Pawn )
{
	EnterState( Pawn );
}

/*
Locked Implementation
*/

GameSpectatorPawn_State::Locked GameSpectatorPawn_State::Locked::Singleton;

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::EnterState( AGameSpectatorPawn* Pawn )
{
	if ( Pawn->GetCurrentState() != &Singleton )
	{
		Pawn->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
const GameSpectatorPawn_State::Locked* GameSpectatorPawn_State::Locked::GetState()
{
	return &Singleton;
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::MoveForward( AGameSpectatorPawn* Pawn, float Val )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::MoveRight( AGameSpectatorPawn* Pawn, float Val )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::MoveUp_World( AGameSpectatorPawn* Pawn, float Val )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::TurnAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate )
{
}

//-----------------------------------------------------------------------------------------------
void GameSpectatorPawn_State::Locked::OrbitSelectedActor( AGameSpectatorPawn* Pawn )
{
}

#pragma endregion