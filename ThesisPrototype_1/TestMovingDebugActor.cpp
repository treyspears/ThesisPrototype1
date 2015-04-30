// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "TestMovingDebugActor.h"


// Sets default values
ATestMovingDebugActor::ATestMovingDebugActor()
	: DecreasingTestNum( FMath::RandRange( 10, 100 ) )
	, Speed( 100.f )
	, VelocityDirection( FVector::ForwardVector )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupPrimitives();

}

//-----------------------------------------------------------------------------------------------
void ATestMovingDebugActor::SetupPrimitives()
{
	BaseCollisionComponent = CreateDefaultSubobject< USphereComponent >( "BaseSphereComponent" );
	TestActorMesh = CreateDefaultSubobject< UStaticMeshComponent >( "TestActorStaticMeshComponent" );

	RootComponent = BaseCollisionComponent;
	BaseCollisionComponent->bGenerateOverlapEvents = true;

	UStaticMesh* coneStaticMesh = nullptr;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshObj( TEXT( "StaticMesh'/Engine/BasicShapes/Sphere.Sphere'" ) );
	if ( staticMeshObj.Succeeded() )
	{
		coneStaticMesh = staticMeshObj.Object;
	}

	TestActorMesh->SetStaticMesh( coneStaticMesh );
	TestActorMesh->AttachTo( BaseCollisionComponent );
}

// Called when the game starts or when spawned
void ATestMovingDebugActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATestMovingDebugActor::Tick( float DeltaTime )
{
	static float ElapsedTime = 0.f;

	Super::Tick( DeltaTime );

	FVector NewLocation = GetActorLocation();

	NewLocation += VelocityDirection * ( Speed * DeltaTime );
	SetActorLocation( NewLocation );

	static float  lapTime = 0.f;
	static float  lapTime2 = 0.f;

	if ( lapTime >= 1.f && DecreasingTestNum > 0 )
	{
		SetDecreasingTestNum( DecreasingTestNum - 1 );
		lapTime = 0.f;
	}

	if ( lapTime2 >= 1.25f )
	{
		SetDebugFlag( !DebugFlag );
		lapTime2 = 0.f;
	}

	lapTime += DeltaTime;
	lapTime2 += DeltaTime;

	ElapsedTime += DeltaTime;
}

//-----------------------------------------------------------------------------------------------
int32 ATestMovingDebugActor::GetDecreasingTestNum() const
{
	return DecreasingTestNum;
}

//-----------------------------------------------------------------------------------------------
void ATestMovingDebugActor::SetDecreasingTestNum( int32 newNum )
{
	DecreasingTestNum = newNum;

	OnDebugNumChanged.Broadcast( DecreasingTestNum );
	//Broadcast event
}

//-----------------------------------------------------------------------------------------------
bool ATestMovingDebugActor::GetDebugFlag() const
{
	return DebugFlag;
}

//-----------------------------------------------------------------------------------------------
void ATestMovingDebugActor::SetDebugFlag( bool newFlag )
{
	DebugFlag = newFlag;

	//Broadcast event
}
