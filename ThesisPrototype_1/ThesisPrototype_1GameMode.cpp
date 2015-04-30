// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "ThesisPrototype_1GameMode.h"

#include "GameSpectatorPawn.h"
#include "GamePlayerController.h"
#include "GameHUD.h"


//-----------------------------------------------------------------------------------------------
AThesisPrototype_1GameMode::AThesisPrototype_1GameMode( const FObjectInitializer& objectInitializer )
	: Super( objectInitializer )
{
	DefaultPawnClass		= AGameSpectatorPawn::StaticClass();
	PlayerControllerClass	= AGamePlayerController::StaticClass();
	HUDClass				= AGameHUD::StaticClass();

}