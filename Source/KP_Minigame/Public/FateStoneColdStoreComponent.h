// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FateStoneColdStoreComponent.generated.h"

class UFateStoneDataAsset;

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FFateStoneToGameData
{
	GENERATED_BODY();
};

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

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UFateStoneDataAsset> SendToGame(int32 index);

	UFUNCTION(BlueprintCallable)
	void ReturnInStore(const TSoftObjectPtr<UFateStoneDataAsset>& FateStone);

	UFUNCTION(BlueprintCallable)
	void AddFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData);

	UFUNCTION(BlueprintCallable)
	bool RemoveFateStone(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData);

	UFUNCTION(BlueprintCallable)
	void SincPlayerStonesData();

	UFUNCTION(BlueprintCallable)
	const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& GetPlayerFateStonesForGame() const;

	//Need save in player's profile
	UPROPERTY(EditDefaultsOnly, Category = ColdStore)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > PlayerFateStones;

	UPROPERTY(BlueprintAssignable)
	FOnUpdatePlayerStore OnUpdatePlayerStore;

protected:
	// data only game;
	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > PlayerFateStonesForGame;
};
