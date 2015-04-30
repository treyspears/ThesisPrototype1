// Fill out your copyright notice in the Description page of Project Settings.

#include "ThesisPrototype_1.h"
#include "MyGameInstance.h"

/*
//-----------------------------------------------------------------------------------------------
FConstAdvertismentComponentsIterator UMyGameInstance::GetAdvertisementComponentsIterator() const
{
	return AdvertismentComponentsList.CreateConstIterator();
}

//-----------------------------------------------------------------------------------------------
int32 UMyGameInstance::GetAdvertisementComponents() const
{
	return AdvertismentComponentsList.Num();
}

//-----------------------------------------------------------------------------------------------
void UMyGameInstance::AddAdvertisementComponent( UAdvertisementComponent* Component )
{
	check( Component );
	AdvertismentComponentsList.AddUnique( Component );
}

//-----------------------------------------------------------------------------------------------
void UMyGameInstance::RemoveAdvertisementComponent( UAdvertisementComponent* Component )
{
	check( Component );

	AdvertismentComponentsList.Remove( Component );
}

*/


//-----------------------------------------------------------------------------------------------
FConstActorsWithAdvertismentIterator UMyGameInstance::GetActorsWithAdvertisementsIterator() const
{
	return ActorsWithAdvertismentsList.CreateConstIterator();
}

//-----------------------------------------------------------------------------------------------
int32 UMyGameInstance::GetNumActorsWithAdvertisements() const
{
	return ActorsWithAdvertismentsList.Num();
}

//-----------------------------------------------------------------------------------------------
void UMyGameInstance::AddActorWithAdvertisement( AActor* Actor )
{
	check( Actor );
	ActorsWithAdvertismentsList.AddUnique( Actor );
}

//-----------------------------------------------------------------------------------------------
void UMyGameInstance::RemoveActorWithAdvertisement( AActor* Actor )
{
	check( Actor );

	ActorsWithAdvertismentsList.Remove( Actor );
}
