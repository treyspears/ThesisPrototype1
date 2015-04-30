// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "NeedsEnum.h"
#include "PersonalityComponent.generated.h"


USTRUCT()
struct FTagAndDispositionModifierPair
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	FName Tag;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	int32 DispositionModifer;
};


USTRUCT( Blueprintable )
struct FPersonalityDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	//UPROPERTY( EditAnywhere, Category = "Utility Curves" )
	//TAssetPtr< UCurveFloat > UtilityCurvesForNeeds[ static_cast< uint8 >( ENeedsEnum::Type::NUM_NEEDS ) ];

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	//TArray< TAssetPtr< UCurveFloat > > UtilityCurvesForNeeds;

	/*UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > MentalNeeds1UtilityCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > MentalNeeds2UtilityCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > MentalNeeds3UtilityCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > PhysicalNeeds1UtilityCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > PhysicalNeeds2UtilityCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > PhysicalNeeds3UtilityCurve;*/

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	FTagAndDispositionModifierPair DispositionModifier;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	//TArray< FTagAndDispositionModifierPair > DispositionModifiers;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESISPROTOTYPE_1_API UPersonalityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPersonalityComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY( EditAnywhere, Category = "Utility Curves" )
	TAssetPtr< UCurveFloat > UtilityCurvesForNeeds[ static_cast< uint8 >( ENeedsEnum::Type::NUM_NEEDS ) ];

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tags" )
	TArray< FTagAndDispositionModifierPair > DispositionModifiers;

	/*
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Personality Data" )
	UDataTable* UtilityCurvesAndDispositionDataTable;	
	*/
};
