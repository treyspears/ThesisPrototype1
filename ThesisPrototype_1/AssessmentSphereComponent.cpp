// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "AssessmentSphereComponent.h"
#include "DebugDataObject.h"
#include "TestMovingDebugActor.h"

//-----------------------------------------------------------------------------------------------
UAssessmentSphereComponent::UAssessmentSphereComponent( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, bIsActive( true )
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
	
	bGenerateOverlapEvents = true;

	OnComponentBeginOverlap.AddDynamic( this, &UAssessmentSphereComponent::OnBeginOverlap );
	OnComponentEndOverlap.AddDynamic( this, &UAssessmentSphereComponent::OnEndOverlap );
}

//-----------------------------------------------------------------------------------------------
UAssessmentSphereComponent::~UAssessmentSphereComponent()
{
	DebugObjectRegistry.Empty();
}

//-----------------------------------------------------------------------------------------------
void UAssessmentSphereComponent::ActivateAssessor()
{
	bIsActive = true;
	bGenerateOverlapEvents = true;
}

//-----------------------------------------------------------------------------------------------
void UAssessmentSphereComponent::DeactivateAssessor()
{
	if ( !bGenerateOverlapEvents )
	{
		return;
	}

	bIsActive = false;
	bGenerateOverlapEvents = false;

	DebugObjectRegistry.Empty();
}

//-----------------------------------------------------------------------------------------------
void UAssessmentSphereComponent::OnBeginOverlap( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
											  , bool FromSweep, const FHitResult& SweepResult )
{
	ATestMovingDebugActor* OtherActorMovingActor = Cast< ATestMovingDebugActor >( OtherActor );

	if ( OtherActorMovingActor )
	{
		UDebugDataObject** FindResult = nullptr;

		FindResult = DebugObjectRegistry.Find( OtherActorMovingActor );

		if ( !FindResult )
		{
			UDebugDataObject* NewDebugTestObject = nullptr;

			NewDebugTestObject = ConstructObject< UDebugDataObject >( UDebugDataObject::StaticClass() );

			NewDebugTestObject->DebugTextToDisplay = FString::FromInt( OtherActorMovingActor->DecreasingTestNum );
			DebugObjectRegistry.Add( OtherActorMovingActor, NewDebugTestObject );

			NewDebugTestObject->OnDebugNumChanged( OtherActorMovingActor->DecreasingTestNum );
			OtherActorMovingActor->OnDebugNumChanged.AddDynamic( NewDebugTestObject, &UDebugDataObject::OnDebugNumChanged );
		}
	}

}

//-----------------------------------------------------------------------------------------------
void UAssessmentSphereComponent::OnEndOverlap( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	ATestMovingDebugActor* OtherActorMovingActor = Cast< ATestMovingDebugActor >( OtherActor );

	if ( OtherActorMovingActor )
	{
		UDebugDataObject* OldDebugTestObject = nullptr;

		DebugObjectRegistry.RemoveAndCopyValue( OtherActorMovingActor, OldDebugTestObject );

		if ( OldDebugTestObject )
		{
			OtherActorMovingActor->OnDebugNumChanged.RemoveDynamic( OldDebugTestObject, &UDebugDataObject::OnDebugNumChanged );
		}
	}
}