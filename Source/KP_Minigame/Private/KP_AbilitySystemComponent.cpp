// Fill out your copyright notice in the Description page of Project Settings.


#include "KP_AbilitySystemComponent.h"
#include "GameplayEffectFateStone.h"

TArray<FActiveTurnBasedEffectHandle> UKP_AbilitySystemComponent::ActiveTurnBasedEffects;

FActiveGameplayEffectHandle UKP_AbilitySystemComponent::ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, UAbilitySystemComponent* Target, FPredictionKey PredictionKey)
{
	FActiveGameplayEffectHandle ActiveEffectHandle = Super::ApplyGameplayEffectSpecToTarget(GameplayEffect, Target, PredictionKey);
	// Register ActiveEffectHandle
	const UGameplayEffectFateStone* FateStoneEffect = Cast<UGameplayEffectFateStone>(GameplayEffect.Def);
	// to do StepsCounter
	AddActiveEffectToTurnBasedEffects(ActiveEffectHandle, FateStoneEffect ? FateStoneEffect->Turns : 6);
	return ActiveEffectHandle;
}

void UKP_AbilitySystemComponent::AddActiveEffectToTurnBasedEffects(const FActiveGameplayEffectHandle& ActiveEffectHandle, const int32 Turns)
{
	FActiveTurnBasedEffectHandle NewHandle = FActiveTurnBasedEffectHandle();
	NewHandle.ActiveEffectHandle = ActiveEffectHandle;
	NewHandle.TurnsLeft = Turns;
	UKP_AbilitySystemComponent::ActiveTurnBasedEffects.Add(NewHandle);
}

void UKP_AbilitySystemComponent::DecreaseActiveTurnBasedEffectsCounters()
{
	// Decrease TurnsLeft for every active effect in the list
	for (FActiveTurnBasedEffectHandle& ActiveTurnBasedEffect : UKP_AbilitySystemComponent::ActiveTurnBasedEffects) 
	{
		// Check if the effect is not permanent
		if (ActiveTurnBasedEffect.TurnsLeft != -1)
		{
			ActiveTurnBasedEffect.TurnsLeft -= 1;
			// Deactivate effect, if expired
			if (ActiveTurnBasedEffect.TurnsLeft == 0) 
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ActiveTurnBasedEffect.ActiveEffectHandle;
				if (ActiveGameplayEffectHandle.IsValid())
				{
					UAbilitySystemComponent* ASC = ActiveGameplayEffectHandle.GetOwningAbilitySystemComponent();
					if (ASC) 
					{
						ASC->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
					}
				}
			}
		}
	}
	// Remove all of the deactived effects
	UKP_AbilitySystemComponent::ActiveTurnBasedEffects.RemoveAll([](const FActiveTurnBasedEffectHandle& ActiveTurnBasedEffect) 
	{
		return not ActiveTurnBasedEffect.ActiveEffectHandle.IsValid();
	});
}
