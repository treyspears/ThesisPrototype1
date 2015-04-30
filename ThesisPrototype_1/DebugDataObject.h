// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DebugDataObject.generated.h"

/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API UDebugDataObject : public UObject
{
	GENERATED_BODY()
	
public:

	FString DebugTextToDisplay;
	
	UFUNCTION()
	void OnDebugNumChanged( int32 NewDebugNum );
};