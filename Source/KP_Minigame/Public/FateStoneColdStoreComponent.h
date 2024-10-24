// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FateStoneColdStoreComponent.generated.h"

class UFateStoneDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatePlayerStore);


//stored all user's Stones Of Fate
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KP_MINIGAME_API UFateStoneColdStoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFateStoneColdStoreComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// for visualization 
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UFateStoneDataAsset> SendToGame(const int32 index);
	// for visualization 
	UFUNCTION(BlueprintCallable)
	void ReturnInStore(const TSoftObjectPtr<UFateStoneDataAsset>& FateStone);

	//work witch player's stones( save game)
	UFUNCTION(BlueprintCallable)
	void AddFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData);

	//work witch player's stones( save game)
	UFUNCTION(BlueprintCallable)
	bool RemoveFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData);

	//Synchronize player's stones and visualization data 
	UFUNCTION(BlueprintCallable)
	void SyncPlayerStonesData();

	UFUNCTION(BlueprintCallable)
	const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& GetPlayerFateStonesForGame() const;

	UPROPERTY(BlueprintAssignable)
	FOnUpdatePlayerStore OnUpdatePlayerStore;

protected:
	// visualization data ;
	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > PlayerFateStonesForGame;

	//Need save in player's profile
	UPROPERTY(EditDefaultsOnly, Category = ColdStore)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > PlayerFateStones;
};
