// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "DebugDataObject.h"

//-----------------------------------------------------------------------------------------------
void UDebugDataObject::OnDebugNumChanged( int32 NewDebugNum )
{
	DebugTextToDisplay = FString::FromInt( NewDebugNum );
}


