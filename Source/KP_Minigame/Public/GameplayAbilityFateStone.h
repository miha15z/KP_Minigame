// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityFateStone.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UGameplayAbilityFateStone : public UGameplayAbility
{
	GENERATED_BODY()
public:
};


// Data Asset that stores a Fate Stone configuration
UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UFateStoneData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbilityFateStone> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

};
