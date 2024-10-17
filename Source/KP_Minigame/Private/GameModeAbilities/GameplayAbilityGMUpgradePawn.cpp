// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMUpgradePawn.h"
#include "KP_GameplayTags.h"

UGameplayAbilityGMUpgradePawn::UGameplayAbilityGMUpgradePawn(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_UpgradePawn;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMUpgradePawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData);

	// Create new pawn of upgraded type

	// Replace the existing pawn woth the newe one

	// Replace all outer references to the old pawn with references to the new one
}


