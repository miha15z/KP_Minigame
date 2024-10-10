// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Core/KP_Structs.h"
#include "KP_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UKP_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, UAbilitySystemComponent* Target, FPredictionKey PredictionKey = FPredictionKey());

protected:
// turn-based effect handling
	static TArray<FActiveTurnBasedEffectHandle> ActiveTurnBasedEffects;

protected:
	static void AddActiveEffectToTurnBasedEffects(FActiveGameplayEffectHandle ActiveEffectHandle, int32 Turns);

	static void DecreaseActiveTurnBasedEffectsCounters();
};
