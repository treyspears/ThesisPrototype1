// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "GamePlayerController.h"
#include "GameSpectatorPawn.h"
#include "GameHUD.h"

//-----------------------------------------------------------------------------------------------
AGamePlayerController::AGamePlayerController( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, bTargetLocked( false )
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::None;
	

	GamePlayerController_State::FirstPerson::EnterState( this );
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::BeginPlay()
{
	APawn* PossessedPawn = GetPawnOrSpectator();

	if ( PossessedPawn )
	{
		AGameSpectatorPawn* SpectatorPawn = Cast< AGameSpectatorPawn >( PossessedPawn );

		if ( SpectatorPawn )
		{		
			OnNewMouseHitResult.AddDynamic( SpectatorPawn, &AGameSpectatorPawn::OnNewMouseHitResult );
			PawnAsSpectatorPawn = SpectatorPawn;
		}
	}

	AGameHUD* TheHUD = Cast< AGameHUD >( MyHUD );

	if( TheHUD )
	{
		OnNewMouseHitResult.AddDynamic( TheHUD, &AGameHUD::OnNewMouseHitResult );
	}
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::UpdateRotation( float DeltaTime )
{
	CurrentState->UpdateRotation( this, DeltaTime );
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction( "SelectActor", IE_Pressed, this, &AGamePlayerController::OnSelectActorPressed );
	InputComponent->BindAction( "SelectActor", IE_Released, this, &AGamePlayerController::OnSelectActorReleased );

	InputComponent->BindAction( "EnterFirstPersonView", IE_Released, this, &AGamePlayerController::OnEnterFirstPersonView );
	InputComponent->BindAction( "EnterTopDownView", IE_Released, this, &AGamePlayerController::OnEnterTopDownView );

	InputComponent->BindAction( "OrbitSelectedActor", IE_Pressed, this, &AGamePlayerController::OnOrbitSelectedActor );
	InputComponent->BindAction( "ToggleLockSelectedActor", IE_Pressed, this, &AGamePlayerController::OnToggleLockSelectedActor );
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnSelectActorPressed()
{
	CurrentState->OnSelectActorPressed( this );
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnSelectActorReleased()
{
	CurrentState->OnSelectActorReleased( this );	
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnEnterFirstPersonView()
{
	GamePlayerController_State::FirstPerson::EnterState( this );

	if ( PawnAsSpectatorPawn.IsValid() )
	{
		PawnAsSpectatorPawn->OnEnterFirstPersonView();
	}
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnEnterTopDownView()
{
	GamePlayerController_State::TopDown::EnterState( this );

	if ( PawnAsSpectatorPawn.IsValid() )
	{
		PawnAsSpectatorPawn->OnEnterTopDownView();
	}
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnOrbitSelectedActor()
{
	if ( PawnAsSpectatorPawn.IsValid() )
	{
		PawnAsSpectatorPawn->OrbitSelectedActor( CurrentMouseClickResult );
	}
}

//-----------------------------------------------------------------------------------------------
void AGamePlayerController::OnToggleLockSelectedActor()
{
	if ( !CurrentMouseClickResult.Actor.IsValid() )
	{
		bTargetLocked = false;
	}
	else
	{
		bTargetLocked = !bTargetLocked;
	}
}

#pragma region AGamePlayerController States

/*
	FirstPerson Implementation
*/

GamePlayerController_State::FirstPerson GamePlayerController_State::FirstPerson::Singleton;

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::FirstPerson::EnterState( AGamePlayerController* PlayerController )
{
	if ( PlayerController->GetCurrentState() != &Singleton )
	{
		PlayerController->bShowMouseCursor = false;

		TWeakObjectPtr< AGameSpectatorPawn > PCPawnAsGameSpectatorPawn = nullptr;

		if ( PlayerController->PawnAsSpectatorPawn.IsValid() )
		{
			PCPawnAsGameSpectatorPawn = PlayerController->PawnAsSpectatorPawn;
			ACameraActor* TopDownCamera = PCPawnAsGameSpectatorPawn->GetTopDownCameraActor();
			
			FRotator NewRotation = PCPawnAsGameSpectatorPawn->TopDownCameraRotation;

			const FRotator SupposedRotation = FRotator( -90.f, 90.f, 0.f );

			PlayerController->SetControlRotation( NewRotation );
		}

		PlayerController->SetViewTargetWithBlend( PlayerController->GetPawn(), 0.f );

		PlayerController->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::FirstPerson::OnSelectActorPressed( AGamePlayerController* PlayerController )
{

}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::FirstPerson::OnSelectActorReleased( AGamePlayerController* PlayerController )
{
	if ( PlayerController->IsTargetLocked() && PlayerController->CurrentMouseClickResult.Actor.IsValid() )
	{
		return;
	}
	else
	{
		PlayerController->SetTargetLocked( false );
	}

	ULocalPlayer* LocalPlayer = Cast< ULocalPlayer>( PlayerController->Player );

	if ( LocalPlayer )
	{
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;

		const FIntPoint HalfViewPortSize = ( Viewport->GetSizeXY() ) / 2;

		FIntPoint PreviousMousePosition = FIntPoint::ZeroValue;
		Viewport->GetMousePos( PreviousMousePosition );

		Viewport->SetMouse( HalfViewPortSize.X, HalfViewPortSize.Y );
		
		PlayerController->GetHitResultUnderCursor( ECC_Visibility, false, PlayerController->CurrentMouseClickResult );

		PlayerController->OnNewMouseHitResult.Broadcast( PlayerController->CurrentMouseClickResult );

		Viewport->SetMouse( PreviousMousePosition.X, PreviousMousePosition.Y );
	}
}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::FirstPerson::UpdateRotation( AGamePlayerController* PlayerController, float DeltaTime )
{
	PlayerController->APlayerController::UpdateRotation( DeltaTime );
}


/*
	TopDown Implementation
*/

GamePlayerController_State::TopDown GamePlayerController_State::TopDown::Singleton;

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::TopDown::EnterState( AGamePlayerController* PlayerController )
{
	if ( PlayerController->GetCurrentState() != &Singleton )
	{
		PlayerController->bShowMouseCursor = true;

		ULocalPlayer* LocalPlayer = Cast< ULocalPlayer>( PlayerController->Player );

		if ( LocalPlayer )
		{
			FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;

			const FIntPoint HalfViewPortSize = ( Viewport->GetSizeXY() ) / 2;
			Viewport->SetMouse( HalfViewPortSize.X, HalfViewPortSize.Y );
		}

		TWeakObjectPtr< AGameSpectatorPawn > PCPawnAsGameSpectatorPawn = nullptr;

		if( PlayerController->PawnAsSpectatorPawn.IsValid() )
		{		
			ACameraActor* TopDownCameraActor = nullptr;
			
			PCPawnAsGameSpectatorPawn = PlayerController->PawnAsSpectatorPawn;
			TopDownCameraActor = PCPawnAsGameSpectatorPawn->GetTopDownCameraActor();

			PlayerController->SetViewTargetWithBlend( TopDownCameraActor );
		}

		PlayerController->SetState( &Singleton );
	}
}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::TopDown::OnSelectActorPressed( AGamePlayerController* PlayerController )
{

}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::TopDown::OnSelectActorReleased( AGamePlayerController* PlayerController )
{
	if ( PlayerController->IsTargetLocked() && PlayerController->CurrentMouseClickResult.Actor.IsValid() )
	{
		return;
	}
	else
	{
		PlayerController->SetTargetLocked( false );
	}

	PlayerController->GetHitResultUnderCursor( ECC_Visibility, false, PlayerController->CurrentMouseClickResult );

	PlayerController->OnNewMouseHitResult.Broadcast( PlayerController->CurrentMouseClickResult );
}

//-----------------------------------------------------------------------------------------------
void GamePlayerController_State::TopDown::UpdateRotation( AGamePlayerController* PlayerController, float DeltaTime )
{

}

#pragma endregion