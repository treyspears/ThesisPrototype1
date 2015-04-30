// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"



/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AGameHUD( const FObjectInitializer& ObjectInitializer );

	virtual void DrawHUD() override;

	void PrintDebugTextAtAllTestActorLocations();

	UFUNCTION()
	void OnNewMouseHitResult( const FHitResult& NewMouseHitResult );


	UPROPERTY()
	UFont* HUDFont;

	FString TestActorClickedNameAsString;

	TWeakObjectPtr< AActor > CurrentSelectedTarget;
	
	/** Material for the crosshair. Has a default if not set in editor/blueprint. */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Crosshair" )
	UMaterial* CrossHairMaterial;

private:

	void DrawTargettedActorInformation();
	void DrawCustomCrosshair();
	void DrawCrosshairAtCenterOfScreen( class APlayerController* PlayerController );
	void DrawCrosshairAtMousePosition( class APlayerController* PlayerController );
};
