// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "NeedsEnum.generated.h"


namespace ENeedsEnum
{
	UENUM()
	enum class THESISPROTOTYPE_1_API Type : uint8
	{
		MentalNeed1,
		MentalNeed2,
		MentalNeed3,
		PhysicalNeed1,
		PhysicalNeed2,
		PhysicalNeed3,

		NUM_NEEDS = 6
	};
}


/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API UNeedsEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};
