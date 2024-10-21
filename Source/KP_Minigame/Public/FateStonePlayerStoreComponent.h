// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FateStonePlayerStoreComponent.generated.h"

class UFateStoneDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateFateStoneStore);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KP_MINIGAME_API UFateStonePlayerStoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFateStonePlayerStoreComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = Store)
	int32 MaxStoreSize = 4;

	UPROPERTY(BlueprintAssignable, Category = Store)
	FOnUpdateFateStoneStore OnUpdateFateStoneStore;

	UFUNCTION(BlueprintCallable, Category = Store)
	bool TryAddFateStone(UFateStoneDataAsset* StoneData);

	UFUNCTION(BlueprintCallable, Category = Store)
	UFateStoneDataAsset* GiveFateStone(const int32 Id);

	UFUNCTION(BlueprintCallable, Category = Store)
	void Init( const TArray<TSoftObjectPtr<UFateStoneDataAsset> >&  InitData);

	UFUNCTION(BlueprintCallable, Category = Store)
	const TArray<UFateStoneDataAsset*>& GetAllStones() const;

	UFUNCTION(BlueprintPure, Category = Store)
	bool IsFull() const;

	UFUNCTION(BlueprintCallable, Category = Store)
	UFateStoneDataAsset* TryUseFateStone(const int32 Id);

	UFUNCTION(BlueprintPure, Category = Store)
	bool CanUseFateStone() const;

	UFUNCTION(BlueprintCallable, Category = Store)
	void ResetNumberOfUse();

	UFUNCTION(BlueprintCallable, Category = Store)
	const UFateStoneDataAsset* GetFateStone(const int32 Id);

	UFUNCTION(BlueprintCallable, Category = Store)
	int32 GetCurrentNumberOfUsePerTurn();

	UFUNCTION(BlueprintCallable, Category = Store)
	int32 GetCurrentNumberOfUseLeft();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Gameplay)
	int32 MaxNumberOfUsesPerTurn = 1;

	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere, Category = Gameplay)
	int32 CurrentNumberOfUsePerTurn = 0;
private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UFateStoneDataAsset>> OwnedFateStones;
};
