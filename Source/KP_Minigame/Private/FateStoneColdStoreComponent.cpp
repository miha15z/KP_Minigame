// Fill out your copyright notice in the Description page of Project Settings.


#include "FateStoneColdStoreComponent.h"

// Sets default values for this component's properties
UFateStoneColdStoreComponent::UFateStoneColdStoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFateStoneColdStoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SyncPlayerStonesData();
}


// Called every frame
void UFateStoneColdStoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TSoftObjectPtr<UFateStoneDataAsset> UFateStoneColdStoreComponent::SendToGame(const int32 index)
{
	check(index >= 0 && index < PlayerFateStonesForGame.Num());
	auto Item = PlayerFateStonesForGame[index];
	PlayerFateStonesForGame.RemoveAt(index);
	OnUpdatePlayerStore.Broadcast();
	return Item;
}

void UFateStoneColdStoreComponent::ReturnInStore(const TSoftObjectPtr<UFateStoneDataAsset>& Item)
{
	PlayerFateStonesForGame.Add(Item);
	OnUpdatePlayerStore.Broadcast();
}

void UFateStoneColdStoreComponent::AddFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData)
{
	PlayerFateStones.Add(FateStoneData);
	SyncPlayerStonesData();
}

bool UFateStoneColdStoreComponent::RemoveFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData)
{
	int32  RemoveNuber = PlayerFateStones.RemoveSingle(FateStoneData);
	SyncPlayerStonesData();
	return RemoveNuber == 1;
}

void UFateStoneColdStoreComponent::SyncPlayerStonesData()
{
	PlayerFateStonesForGame = PlayerFateStones;
	OnUpdatePlayerStore.Broadcast();
}

const TArray<TSoftObjectPtr<UFateStoneDataAsset>>& UFateStoneColdStoreComponent::GetPlayerFateStonesForGame() const
{
	return PlayerFateStonesForGame;
}

