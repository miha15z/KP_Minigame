// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityFateStone.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UGameplayAbilityFateStone : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="Fate Stone")
	TArray<TSubclassOf<AActor>> TargetClasses;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanUseFateStone(AActor* Target, AActor* Owner, int32 FateStoneActions = 0);
//	virtual bool CanUseFateStone_Implementation(AActor* Target, AActor* Owner, int32 FateStoneActions);

	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<AActor>> GetTargetClasses();

};


// Data Asset that stores a Fate Stone configuration
UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UFateStoneData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbilityFateStone> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> IconUI = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//UMaterialInterface* CellMaterial = nullptr;

	// to do, work radius

public:
	UClass* GetGameplayAbilityClass() const;

	UTexture2D* GetIconTexture() const;

	//UMaterialInterface* GetCellMaterial() const;

};
