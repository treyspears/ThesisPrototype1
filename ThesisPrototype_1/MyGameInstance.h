// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

typedef TArray<TAutoWeakObjectPtr<AActor> >::TConstIterator FConstActorsWithAdvertismentIterator;
//typedef TArray<TAutoWeakObjectPtr<UAdvertisementComponent> >::TConstIterator FConstAdvertismentComponentsIterator;

/**
 * 
 */
UCLASS()
class THESISPROTOTYPE_1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/** @return Returns an iterator for the actors with advertisement components list. */
	FConstActorsWithAdvertismentIterator GetActorsWithAdvertisementsIterator() const;
	//FConstAdvertismentComponentsIterator GetAdvertisementComponentsIterator() const;

	/** @return Returns the number of actors with advertisement component instances in world. */
	//int32 GetAdvertisementComponents() const;
	int32 GetNumActorsWithAdvertisements() const;

	/**
	* Inserts the passed in actor with advertisement component at the front of the linked list of actors with advertisement components.
	*
	* @param	Actor	actor with advertisement component to insert, use NULL to clear list
	*/
	void AddActorWithAdvertisement( AActor* Actor );
	//void AddAdvertisementComponent( UAdvertisementComponent* Component );
	

	/**
	* Removes the passed in actor with advertisement component from the linked list of actors with advertisement components.
	*
	* @param	Actor	actor with advertisement component to remove
	*/
	void RemoveActorWithAdvertisement( AActor* Actor );
	//void RemoveAdvertisementComponent( UAdvertisementComponent* Component );
	

private:

	/** List of all the instanced actors with advertisement components in the world. */
	TArray<TAutoWeakObjectPtr<class AActor> > ActorsWithAdvertismentsList;
	//TArray<TAutoWeakObjectPtr<class UAdvertisementComponent> > AdvertismentComponentsList;
	
		
};