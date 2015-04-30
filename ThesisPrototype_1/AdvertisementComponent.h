// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "NeedsEnum.h"
#include "AdvertisementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESISPROTOTYPE_1_API UAdvertisementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAdvertisementComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	/** Called when a component is destroyed */
	virtual void OnComponentDestroyed() override;

	// Tags of who ( i.e. wolves ) or traits ( i.e. neat freak ) of entities this advertisement would appeal to.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	TArray< FName > AppealsToTags;

	// Traits a potential interactor must have to be able to even consider this advertisement. 
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	TArray< FName > RequirementsTags;

	// Mental and Physical needs this advertisement gives when interacted with.
	UPROPERTY( EditAnywhere, Category = "NeedsGivenWhenInteractedWith" )
	int32 MentalAndPhysicalNeeds[ static_cast< uint8 >( ENeedsEnum::Type::NUM_NEEDS ) ];
	
	// Behavior Tree should go here with series of actions interactor should take before receiving Mental and/or Physical needs reward.
	
};
