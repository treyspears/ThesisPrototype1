// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "AdvertisementComponent.h"
#include "MyGameInstance.h"

// Sets default values for this component's properties
UAdvertisementComponent::UAdvertisementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

// Called when the game starts
void UAdvertisementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	UWorld* const World = GetWorld();

	if( IsValid( World ) )
	{
		UGameInstance* GameInstance = World->GetGameInstance();

		if( IsValid( GameInstance ) )
		{
			UMyGameInstance* MyGameInstance = Cast< UMyGameInstance >( GameInstance );
			
			//If you're going to use advertisements, the game instance should be set to MyGameInstance.
			check( MyGameInstance );

			AActor* const Owner = GetOwner();

			if ( IsValid( Owner ) )
			{
				MyGameInstance->AddActorWithAdvertisement( Owner );
			}	
		}	
	}
}

//-----------------------------------------------------------------------------------------------
void UAdvertisementComponent::OnComponentDestroyed()
{
	UWorld* const World = GetWorld();

	if ( IsValid( World ) )
	{
		UGameInstance* GameInstance = World->GetGameInstance();

		if ( IsValid( GameInstance ) )
		{
			UMyGameInstance* MyGameInstance = Cast< UMyGameInstance >( GameInstance );

			//If you're going to use advertisements, the game instance should be set to MyGameInstance.
			check( MyGameInstance );

			AActor* const Owner = GetOwner();

			if ( IsValid( Owner ) )
			{
				TArray< UActorComponent* > AdvertisementComponents = Owner->GetComponentsByClass( UAdvertisementComponent::StaticClass() );

				if ( AdvertisementComponents.Num() == 0 )
				{
					MyGameInstance->RemoveActorWithAdvertisement( Owner );
				}
			}
		}
	}

	Super::OnComponentDestroyed();
}

