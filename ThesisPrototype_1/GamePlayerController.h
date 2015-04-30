// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"


class AGameSpectatorPawn;

#pragma region GamePlayerController States

class AGamePlayerController;

namespace GamePlayerController_State
{

	class Base
	{
	public:

		virtual void OnSelectActorPressed( AGamePlayerController* PlayerController ) = 0;
		virtual void OnSelectActorReleased( AGamePlayerController* PlayerController ) = 0;
		virtual void UpdateRotation( AGamePlayerController* PlayerController, float DeltaTime ) = 0;
	};


	class FirstPerson : public GamePlayerController_State::Base
	{
	public:

		virtual void OnSelectActorPressed( AGamePlayerController* PlayerController ) override;
		virtual void OnSelectActorReleased( AGamePlayerController* PlayerController ) override;
		virtual void UpdateRotation( AGamePlayerController* PlayerController, float DeltaTime ) override;

		static void EnterState( AGamePlayerController* PlayerController );

	private:

		static FirstPerson Singleton;

		FirstPerson() {};

		FirstPerson( FirstPerson const& );
		void operator=( FirstPerson const& );
	};


	class TopDown : public GamePlayerController_State::Base
	{
	public:

		virtual void OnSelectActorPressed( AGamePlayerController* PlayerController ) override;
		virtual void OnSelectActorReleased( AGamePlayerController* PlayerController ) override;
		virtual void UpdateRotation( AGamePlayerController* PlayerController, float DeltaTime ) override;

		static void EnterState( AGamePlayerController* PlayerController );

	private:

		static TopDown Singleton;

		TopDown() {};

		TopDown( TopDown const& );
		void operator=( TopDown const& );
	};
}

#pragma endregion

//Delegate Signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FNewMouseHitResultSignature, const FHitResult&, NewMouseHitResult );

/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AGamePlayerController( const FObjectInitializer& ObjectInitializer );
	
	virtual void BeginPlay() override;
	virtual void UpdateRotation( float DeltaTime ) override;
	
	void OnSelectActorPressed();
	void OnSelectActorReleased();

	void OnEnterFirstPersonView();
	void OnEnterTopDownView();

	void OnOrbitSelectedActor();
	void OnToggleLockSelectedActor();

	FORCEINLINE void SetTargetLocked( bool bIsTargetLocked );
	FORCEINLINE bool IsTargetLocked() const;

	FORCEINLINE void SetState( class GamePlayerController_State::Base* const NewState );
	FORCEINLINE class GamePlayerController_State::Base const* const GetCurrentState() const;

	FHitResult CurrentMouseClickResult;
	TWeakObjectPtr< AGameSpectatorPawn > PawnAsSpectatorPawn; 

	//Delegates
	FNewMouseHitResultSignature OnNewMouseHitResult;

protected:

	virtual void SetupInputComponent() override;

private:

	class GamePlayerController_State::Base* CurrentState;
	bool bTargetLocked;
};

//-----------------------------------------------------------------------------------------------
FORCEINLINE void AGamePlayerController::SetTargetLocked( bool bIsTargetLocked )
{
	bTargetLocked = bIsTargetLocked;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE bool AGamePlayerController::IsTargetLocked() const
{
	return bTargetLocked;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE void AGamePlayerController::SetState( class GamePlayerController_State::Base* const NewState )
{
	CurrentState = NewState;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE class GamePlayerController_State::Base const* const AGamePlayerController::GetCurrentState() const
{
	return CurrentState;
}