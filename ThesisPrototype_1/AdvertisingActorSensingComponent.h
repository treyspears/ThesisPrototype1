// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "AdvertisingActorSensingComponent.generated.h"

class AActor;

/**
* AdvertisingActorSensingComponent encapsulates sensory (i.e. sight and hearing, currently only sight) settings and functionality for an Actor,
* allowing the actor to see(/hear) Advertising Actors in the world. It does nothing on network clients.
*/
UCLASS( ClassGroup = AI, HideCategories = ( Activation, "Components|Activation", Collision ), meta = ( BlueprintSpawnableComponent ) )
class THESISPROTOTYPE_1_API UAdvertisingActorSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSeeActorDelegate, AActor*, Actor );

	/** Maximum sight distance. */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = AI )
	float SightRadius;

	/** Amount of time between pawn sensing updates. Use SetSensingInterval() to adjust this at runtime. A value <= 0 prevents any updates. */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = AI )
	float SensingInterval;

	// Sets default values for this component's properties
	UAdvertisingActorSensingComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;
	
	/**
	* Changes the SensingInterval.
	* If we are currently waiting for an interval, this can either extend or shorten that interval.
	* A value <= 0 prevents any updates.
	*/
	UFUNCTION( BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Components|ActorSensing" )
	virtual void SetSensingInterval( const float NewSensingInterval );

	/** Enables or disables sensing updates. The timer is reset in either case. */
	UFUNCTION( BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Components|ActorSensing" )
	virtual void SetSensingUpdatesEnabled( const bool bEnabled );

	/** Sets PeripheralVisionAngle. Calculates PeripheralVisionCosine from PeripheralVisionAngle */
	UFUNCTION( BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Components|ActorSensing" )
	virtual void SetPeripheralVisionAngle( const float NewPeripheralVisionAngle );

	UFUNCTION( BlueprintCallable, Category = "AI|Components|ActorSensing" )
	float GetPeripheralVisionAngle() const;

	UFUNCTION( BlueprintCallable, Category = "AI|Components|ActorSensing" )
	float GetPeripheralVisionCosine() const;

	/** If true, component will perform sensing updates. At runtime change this using SetSensingUpdatesEnabled(). */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = AI )
	uint32 bEnableSensingUpdates : 1;

	/** If true, we will perform visibility tests and will trigger notifications when a Actors is visible. Default: true */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = AI )
	uint32 bSeeActors : 1;

	/** Is the given actor our owner? Used to ensure that we are not trying to sense our self / our owner. */
	virtual bool IsSensorActor( const AActor* Actor ) const;

	/** Are we capable of sensing anything (and do we have any callbacks that care about sensing)? If so UpdateAISensing() will be called every sensing interval. */
	virtual bool CanSenseAnything() const;

	/** Returns true if we should check whether the given Actor is visible (because we can see things, the Actor is not hidden) */
	virtual bool ShouldCheckVisibilityOf( AActor* Actor ) const;

	/**
	* Chance of seeing other Actor decreases with increasing distance or angle in peripheral vision
	* @param bMaySkipChecks if true allows checks to be sometimes skipped if target is far away (used by periodic automatic visibility checks)
	* @return true if the specified Actor Other is potentially visible (within peripheral vision, etc.) - still need to do LineOfSightTo() check to see if actually visible.
	*/
	virtual bool CouldSeeActor( const AActor* Other, bool bMaySkipChecks = false ) const;

	/**
	* Check line to other actor.
	* @param Other is the actor whose visibility is being checked.
	* @param ViewPoint is eye position visibility is being checked from.
	* @return true if controller's pawn can see Other actor.
	*/
	virtual bool HasLineOfSightTo( const AActor* Other ) const;

	/** Get position where seeing occurs (i.e. eye position).  If we ever need different positions for seeing, we'll deal with that then! */
	virtual FVector GetSensorLocation() const;

	/**  Get the rotation of this sensor. We need this for the sight component */
	virtual FRotator GetSensorRotation() const;

	/** Delegate to execute when we see a Actor. */
	UPROPERTY( BlueprintAssignable )
	FSeeActorDelegate OnSeeActor;

protected:

	/** See if there are interesting sights that we want to detect, and respond to them if so. */
	virtual void SenseActor( AActor& Actor );

	/** Update function called on timer intervals. */
	virtual void OnTimer();

	/** Handle for efficient management of OnTimer timer */
	FTimerHandle TimerHandle_OnTimer;

	/** Modify the timer to fire in TimeDelay seconds. A value <= 0 disables the timer. */
	virtual void SetTimer( const float TimeDelay );

	/** Calls SenseActor on any Actors that we are allowed to sense. */
	virtual void UpdateAISensing();

	AActor* GetSensorActor() const;	// Get the actor used as the actual sensor location is derived from this actor.
	AController* GetSensorController() const; // Get the controller of the sensor actor.

	// Broadcasts notification that our sensor sees a Actor, using the OnSeeActor delegates.
	virtual void BroadcastOnSeeActor( AActor& Actor );

	/** How far to the side AI can see, in degrees. Use SetPeripheralVisionAngle to change the value at runtime. */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = AI )
	float PeripheralVisionAngle;

	/** Cosine of limits of peripheral vision. Computed from PeripheralVisionAngle. */
	UPROPERTY()
	float PeripheralVisionCosine;
};

//-----------------------------------------------------------------------------------------------
inline float UAdvertisingActorSensingComponent::GetPeripheralVisionAngle() const
{
	return PeripheralVisionAngle;
}

//-----------------------------------------------------------------------------------------------
inline float UAdvertisingActorSensingComponent::GetPeripheralVisionCosine() const
{
	return PeripheralVisionCosine;
}
