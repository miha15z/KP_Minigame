// Fill out your copyright notice in the Description page of Project Settings.


#include "KP_AbilitySystemComponent.h"

FActiveGameplayEffectHandle UKP_AbilitySystemComponent::ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, UAbilitySystemComponent* Target, FPredictionKey PredictionKey)
{
	FActiveGameplayEffectHandle ActiveEffect = UAbilitySystemComponent::ApplyGameplayEffectSpecToTarget(GameplayEffect, Target, PredictionKey);
	// Register ActiveEffect in a singleton
	return FActiveGameplayEffectHandle();
}
