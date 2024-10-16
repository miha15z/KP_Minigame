// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameModeAbility.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KP_MINIGAME_API UGameModeAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = GameplayCue)
	bool bAutoActivateGameplayCue = true;
	
};
