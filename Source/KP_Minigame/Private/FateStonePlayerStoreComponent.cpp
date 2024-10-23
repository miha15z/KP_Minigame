// Fill out your copyright notice in the Description page of Project Settings.


#include "FateStonePlayerStoreComponent.h"
#include "FateStoneDataAsset.h"

// Sets default values for this component's properties
UFateStonePlayerStoreComponent::UFateStonePlayerStoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFateStonePlayerStoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFateStonePlayerStoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFateStonePlayerStoreComponent::TryAddFateStone(UFateStoneDataAsset* StoneData)
{
	if (OwnedFateStones.Num() < MaxStoreSize && StoneData)
	{
		OwnedFateStones.Add(StoneData);
		OnUpdateFateStoneStore.Broadcast();
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Try add StoneData failed, %d contain Stones, Max %d"), OwnedFateStones.Num(), MaxStoreSize);
	return false;
}

UFateStoneDataAsset* UFateStonePlayerStoreComponent::GiveFateStone(const int32 Id)
{
	if (Id >= 0, Id < OwnedFateStones.Num())
	{
		UFateStoneDataAsset* StoneData =  OwnedFateStones[Id];
		OwnedFateStones.RemoveAt(Id);
		OnUpdateFateStoneStore.Broadcast();
		return StoneData;
	}
	return nullptr;
}

void UFateStonePlayerStoreComponent::Init(const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& InitData)
{
	for (auto& StoneData : InitData)
	{
		OwnedFateStones.Add(StoneData.LoadSynchronous());
	}
	UE_LOG(LogTemp, Display, TEXT("Init %d Stones, Max %d"), OwnedFateStones.Num(), MaxStoreSize);
	check(OwnedFateStones.Num() <= MaxStoreSize);
}

const TArray<UFateStoneDataAsset*>& UFateStonePlayerStoreComponent::GetAllStones() const
{
	return OwnedFateStones;
}

bool UFateStonePlayerStoreComponent::IsFull() const
{
	return OwnedFateStones.Num() == MaxStoreSize;
}

UFateStoneDataAsset* UFateStonePlayerStoreComponent::TryUseFateStone(const int32 Id)
{
	if (CanUseFateStone())
	{
		++CurrentNumberOfUsePerTurn;
		return GiveFateStone(Id);
	}
	return nullptr;
}

bool UFateStonePlayerStoreComponent::CanUseFateStone() const
{
	return CurrentNumberOfUsePerTurn < MaxNumberOfUsesPerTurn;
}

void UFateStonePlayerStoreComponent::ResetNumberOfUse()
{
	CurrentNumberOfUsePerTurn = 0;
}

const UFateStoneDataAsset* UFateStonePlayerStoreComponent::GetFateStone(const int32 Id)
{
	if (Id >= 0, Id < OwnedFateStones.Num())
	{
		return OwnedFateStones[Id];
	}
	return nullptr;
}

int32 UFateStonePlayerStoreComponent::GetCurrentNumberOfUsePerTurn()
{
	return CurrentNumberOfUsePerTurn;
}

int32 UFateStonePlayerStoreComponent::GetCurrentNumberOfUseLeft()
{
	return MaxNumberOfUsesPerTurn - CurrentNumberOfUsePerTurn;
}

int32 UFateStonePlayerStoreComponent::Num() const
{
	return OwnedFateStones.Num();
}
