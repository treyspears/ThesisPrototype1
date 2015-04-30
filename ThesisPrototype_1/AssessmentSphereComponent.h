// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "AssessmentSphereComponent.generated.h"

class UDebugDataObject;
class ATestMovingDebugActor;

/**
 * 
 */
UCLASS( ClassGroup = "AI", editinlinenew, hidecategories = ( Object, LOD, Lighting, TextureStreaming ), meta = ( DisplayName = "Assessor Component", BlueprintSpawnableComponent ) )
class THESISPROTOTYPE_1_API UAssessmentSphereComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UAssessmentSphereComponent( const FObjectInitializer& ObjectInitializer );
	~UAssessmentSphereComponent();
	
	void ActivateAssessor();
	void DeactivateAssessor();

	FORCEINLINE bool IsActive() const;

	UFUNCTION()
	void OnBeginOverlap( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
						, bool FromSweep, const FHitResult& SweepResult );

	UFUNCTION()
	void OnEndOverlap( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	FORCEINLINE const TMap< ATestMovingDebugActor*, UDebugDataObject* >& GetDebugDataObjects() const;

private:

	bool bIsActive;

	TMap< ATestMovingDebugActor*, UDebugDataObject* > DebugObjectRegistry;
};

//-----------------------------------------------------------------------------------------------
FORCEINLINE bool UAssessmentSphereComponent::IsActive() const
{
	return bIsActive;
}

//-----------------------------------------------------------------------------------------------
FORCEINLINE const TMap< ATestMovingDebugActor*, UDebugDataObject* >& UAssessmentSphereComponent::GetDebugDataObjects() const
{
	return DebugObjectRegistry;
}