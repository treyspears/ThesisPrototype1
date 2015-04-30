// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "GameSpectatorPawn.generated.h"

class ACameraActor;

#pragma region TestSpectorPawn States

class AGameSpectatorPawn;

namespace GameSpectatorPawn_State
{

	class Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) = 0;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) = 0;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) = 0;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) = 0;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) = 0;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) = 0;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) = 0;

	};


	class FirstPersonFlying : public GameSpectatorPawn_State::Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) override;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) override;

		static void EnterState( AGameSpectatorPawn* Pawn );
		static const FirstPersonFlying* GetState();

	private:

		static FirstPersonFlying Singleton;

		FirstPersonFlying() {};

		FirstPersonFlying( FirstPersonFlying const& );
		void operator=( FirstPersonFlying const& );
	};


	class FirstPersonOrbit : public GameSpectatorPawn_State::Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) override;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) override;

		static void EnterState( AGameSpectatorPawn* Pawn );
		static const FirstPersonOrbit* GetState();

	private:

		struct FActorBounds
		{
			FActorBounds()
				: bIsDirty( true )
				, Origin( 0.f, 0.f, 0.f )
				, BoxExtent( 0.f, 0.f, 0.f )
			{}

			FVector Origin;
			FVector BoxExtent;
			bool bIsDirty;
		};

		static FirstPersonOrbit Singleton;
		static FVector PreviousAttachedTargetPosition;
		static FRotator PreviousViewRotation;
		static float CurrentMinDistanceFromTarget;
		static FActorBounds CurrentTargetBounds;

		FirstPersonOrbit() {};

		FirstPersonOrbit( FirstPersonOrbit const& );
		void operator=( FirstPersonOrbit const& );
	};


	class TopDownUnAttached : public GameSpectatorPawn_State::Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) override;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) override;

		static void EnterState( AGameSpectatorPawn* Pawn );
		static const TopDownUnAttached* GetState();

	private:

		static TopDownUnAttached Singleton;

		TopDownUnAttached() {};

		TopDownUnAttached( TopDownUnAttached const& );
		void operator=( TopDownUnAttached const& );
	};


	class TopDownAttached : public GameSpectatorPawn_State::Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) override;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) override;

		static void EnterState( AGameSpectatorPawn* Pawn );
		static const TopDownAttached* GetState();

	private:

		static TopDownAttached Singleton;
		static FVector PreviousAttachedTargetPosition;

		TopDownAttached() {};

		TopDownAttached( TopDownAttached const& );
		void operator=( TopDownAttached const& );

		
	};


	class Locked : public GameSpectatorPawn_State::Base
	{
	public:

		virtual void Tick( AGameSpectatorPawn* Pawn, float DeltaSeconds ) override;

		virtual void MoveForward( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveRight( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void MoveUp_World( AGameSpectatorPawn* Pawn, float Val ) override;
		virtual void TurnAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void LookUpAtRate( AGameSpectatorPawn* Pawn, float Rate ) override;
		virtual void OrbitSelectedActor( AGameSpectatorPawn* Pawn ) override;

		static void EnterState( AGameSpectatorPawn* Pawn );
		static const Locked* GetState();

	private:

		static Locked Singleton;

		Locked() {};

		Locked( Locked const& );
		void operator=( Locked const& );
	};

}

#pragma endregion

/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API AGameSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:

	AGameSpectatorPawn( const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override; 

	void InitializeCameras();
	void SetViewPitchMinMax( float NewMinAngle, float NewMaxAngle );
	
	virtual void MoveForward( float Val ) override;
	virtual void MoveRight( float Val ) override;
	virtual void MoveUp_World( float Val ) override;
	void TurnAtRate( float Rate );
	void LookUpAtRate( float Rate );
	void OrbitSelectedActor( const FHitResult& CurrentMouseClickResult );

	void OnEnterFirstPersonView();
	void OnEnterTopDownView();

	FORCEINLINE void SetState( class GameSpectatorPawn_State::Base* const NewState );
	FORCEINLINE class GameSpectatorPawn_State::Base const* const GetCurrentState() const;

	FORCEINLINE ACameraActor* GetFirstPersonCameraActor() const; 
	FORCEINLINE ACameraActor* GetTopDownCameraActor() const;
	void MovePawnToTopDownCameraZPosition();

	/** Callback function when a new Mouse Hit Event is thrown by PlayerController */
	UFUNCTION()
	void OnNewMouseHitResult( const FHitResult& NewMouseHitResult );

	/** First person camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UChildActorComponent* FirstPersonCameraComponent;

	/** Top down camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UChildActorComponent* TopDownCameraComponent;

	/** Rotation of the TopDown Camera. */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Camera" )
	FRotator TopDownCameraRotation;
	
	/** Starting Z-Location of Top Down Camera */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Camera )
	float InitialTopDownCameraWorldZLocation;

	/** If we're in orbit mode, the target we're orbiting*/
	UPROPERTY()
	TWeakObjectPtr< AActor > CurrentOrbitTarget;

	/** Cached distance to orbit target, will need to re-compute if zoom-level is changed. */
	UPROPERTY()
	FVector DistanceToOrbitTarget;

protected:

	virtual void SetupPlayerInputComponent( class UInputComponent* InputComponent ) override;

private:

	class GameSpectatorPawn_State::Base* CurrentState;
};

//-----------------------------------------------------------------------------------------------
FORCEINLINE void AGameSpectatorPawn::SetState( class GameSpectatorPawn_State::Base* const NewState )
{
	CurrentState = NewState;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE GameSpectatorPawn_State::Base const* const AGameSpectatorPawn::GetCurrentState() const
{
	return CurrentState;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE ACameraActor* AGameSpectatorPawn::GetFirstPersonCameraActor() const
{
	ACameraActor* result = nullptr;

	result = Cast< ACameraActor >( FirstPersonCameraComponent->ChildActor );

	return result;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE ACameraActor* AGameSpectatorPawn::GetTopDownCameraActor() const
{
	ACameraActor* result = nullptr;

	result = Cast< ACameraActor >( TopDownCameraComponent->ChildActor );

	return result;
}
