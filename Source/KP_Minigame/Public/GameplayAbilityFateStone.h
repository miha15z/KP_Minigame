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
public:
};

UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UFateStoneData : public UDataAsset {
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbilityFateStone> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* IconUI = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//UMaterialInterface* CellMaterial = nullptr;

	// to do, work radius

public:
	UClass* GetGameplayAbilityClass() const;

	UTexture2D* GetIconTexture() const;

	//UMaterialInterface* GetCellMaterial() const;

};
