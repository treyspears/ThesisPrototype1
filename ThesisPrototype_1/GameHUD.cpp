// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "GameHUD.h"
#include "TestMovingDebugActor.h"
#include "AssessmentSphereComponent.h"
#include "DebugDataObject.h"


//-----------------------------------------------------------------------------------------------
AGameHUD::AGameHUD( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, CurrentSelectedTarget( nullptr )
{
	static ConstructorHelpers::FObjectFinder<UFont> FontObj( TEXT( "/Engine/EngineFonts/RobotoDistanceField" ) );

	HUDFont = FontObj.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> CrossHairMatObj( TEXT( "Material'/Game/Content/Materials/DefaultCrosshairMaterial.DefaultCrosshairMaterial'" ) );

	CrossHairMaterial = CrossHairMatObj.Object;
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawTargettedActorInformation();
	DrawCustomCrosshair();
	PrintDebugTextAtAllTestActorLocations();
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::PrintDebugTextAtAllTestActorLocations()
{
	const float SHADOW_OFFSET = 2.f;
	const float SCALE_Z_FACTOR = 100.f;
	const FVector OFFSET_TEXT_POSITION( 35.f, 0.f, 0.f );

	if ( !CurrentSelectedTarget.IsValid() )
	{
		return;
	}

	UAssessmentSphereComponent* AssessmentComponent = Cast< UAssessmentSphereComponent >( CurrentSelectedTarget->GetComponentByClass( UAssessmentSphereComponent::StaticClass() ) );

	if ( !AssessmentComponent )
	{
		return;
	}

	const TMap< ATestMovingDebugActor*, UDebugDataObject* > DebugObjects = AssessmentComponent->GetDebugDataObjects();

	float ScaleFontFactor = 0.f;
	FVector WorldToScreenPositionOfActor = FVector::ZeroVector;

	for ( const TPair< ATestMovingDebugActor*, UDebugDataObject* > DebugObjIter : DebugObjects )
	{
		WorldToScreenPositionOfActor = Canvas->Project( DebugObjIter.Key->GetActorLocation() );

		if ( WorldToScreenPositionOfActor.Z <= 0.f )
			continue;

		ScaleFontFactor = WorldToScreenPositionOfActor.Z * SCALE_Z_FACTOR;
		WorldToScreenPositionOfActor += ( OFFSET_TEXT_POSITION * ScaleFontFactor );

		DrawRect( FLinearColor( 0.0843137254901961f, 0.0843137254901961f, 0.0843137254901961f, 1.f ), WorldToScreenPositionOfActor.X, WorldToScreenPositionOfActor.Y, 100.f * ScaleFontFactor, 100.f * ScaleFontFactor );
		DrawText( DebugObjIter.Value->DebugTextToDisplay, FLinearColor( 0.f, 0.f, 0.f, 1.f ), WorldToScreenPositionOfActor.X + SHADOW_OFFSET, WorldToScreenPositionOfActor.Y + SHADOW_OFFSET, HUDFont, ScaleFontFactor );
		DrawText( DebugObjIter.Value->DebugTextToDisplay, FLinearColor( 0.f, 1.f, 1.f, 1.f ), WorldToScreenPositionOfActor.X, WorldToScreenPositionOfActor.Y, HUDFont, ScaleFontFactor );
	}
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::OnNewMouseHitResult( const FHitResult& NewMouseHitResult )
{
	CurrentSelectedTarget = NewMouseHitResult.Actor;

	if( !NewMouseHitResult.Actor.IsValid() )
	{
		TestActorClickedNameAsString.Empty();
		return;
	}


	TestActorClickedNameAsString = NewMouseHitResult.Actor->GetName();
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::DrawTargettedActorInformation()
{
	if ( !TestActorClickedNameAsString.IsEmpty() )
	{
		const FLinearColor FontColor = FLinearColor::Yellow;
		const FVector2D TestPosition = FVector2D::ZeroVector;

		DrawText( TestActorClickedNameAsString, FontColor, TestPosition.X, TestPosition.Y, HUDFont );
	}
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::DrawCustomCrosshair()
{
	if ( CrossHairMaterial )
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController( GetWorld(), 0 );

		if ( PlayerController )
		{
			if ( PlayerController->bShowMouseCursor )
			{
				DrawCrosshairAtMousePosition( PlayerController );
			}
			else
			{
				DrawCrosshairAtCenterOfScreen( PlayerController );
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::DrawCrosshairAtCenterOfScreen( class APlayerController* PlayerController )
{
	const FVector2D MouseMaterialSize = FVector2D( 32.f, 32.f );

	int32 ViewportX = 0;
	int32 ViewportY = 0;

	PlayerController->GetViewportSize( ViewportX, ViewportY );

	FVector2D MouseOffset = FVector2D( Canvas->SizeX - ViewportX, Canvas->SizeY - ViewportY ) * 0.5f;

	FVector2D MousePosition = FVector2D( Canvas->SizeX, Canvas->SizeY ) * 0.5f;

	MousePosition += MouseOffset;
	MousePosition -= MouseMaterialSize * 0.5f;

	/*
	FCanvasLineItem HorizontalLineItem( FVector2D::ZeroVector, FVector2D( MouseMaterialSize.X, 0.f ) );
	FCanvasLineItem VerticalLineItem( FVector2D::ZeroVector, FVector2D( 0.f, MouseMaterialSize.Y ) );

	HorizontalLineItem.BlendMode = SE_BLEND_TranslucentAlphaOnly;
	VerticalLineItem.BlendMode = SE_BLEND_TranslucentAlphaOnly;

	Canvas->K2_DrawLine()
	HorizontalLineItem.SetColor( FLinearColor::White );
	VerticalLineItem.SetColor( FLinearColor::White );

	Canvas->DrawItem( HorizontalLineItem, MousePosition );
	Canvas->DrawItem( VerticalLineItem, MousePosition );

	*/

	DrawMaterial( CrossHairMaterial, MousePosition.X, MousePosition.Y, MouseMaterialSize.X, MouseMaterialSize.Y, 0.f, 0.f, 1.f, 1.f );	
}

//-----------------------------------------------------------------------------------------------
void AGameHUD::DrawCrosshairAtMousePosition( class APlayerController* PlayerController )
{
	const FVector2D MouseMaterialSize = FVector2D( 64.f, 64.f );

	int32 ViewportX = 0;
	int32 ViewportY = 0;

	PlayerController->GetViewportSize( ViewportX, ViewportY );

	FVector2D MouseOffset( Canvas->SizeX - ViewportX, Canvas->SizeY - ViewportY );
	MouseOffset *= 0.5f;

	FVector2D MousePosition = FVector2D::ZeroVector;
	PlayerController->GetMousePosition( MousePosition.X, MousePosition.Y );

	MousePosition += MouseOffset;
	MousePosition -= MouseMaterialSize * 0.5f;

	DrawMaterial( CrossHairMaterial, MousePosition.X, MousePosition.Y, MouseMaterialSize.X, MouseMaterialSize.Y, 0.f, 0.f, 1.f, 1.f );
}