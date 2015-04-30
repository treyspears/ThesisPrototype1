// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TestMovingDebugActor.generated.h"

class UAssessmentSphereComponent;

//Delegate Signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDebugNumChangedSignature, int32, NewDebugNumValue );

UCLASS()
class THESISPROTOTYPE_1_API ATestMovingDebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestMovingDebugActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	int32 GetDecreasingTestNum() const;
	void  SetDecreasingTestNum( int32 newNum );
	bool  GetDebugFlag() const;
	void  SetDebugFlag( bool newFlag );

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Debug" )
	int32 DecreasingTestNum;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Debug" )
	bool DebugFlag;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float Speed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	FVector VelocityDirection;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision" )
	USphereComponent* BaseCollisionComponent;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Appearance" )
	UStaticMeshComponent* TestActorMesh;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI" )
	UAssessmentSphereComponent* AssessmentComponent;

	//Delegates
	FDebugNumChangedSignature OnDebugNumChanged;

private:

	void SetupPrimitives();
	
};
