// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMTurnEnd.h"
#include "KP_GameplayTags.h"

UGameplayAbilityGMTurnEnd::UGameplayAbilityGMTurnEnd(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	this->AbilityTags.AddTag(KP_GameplayTags::Gameplay_Ability_EndTurn);
}

void UGameplayAbilityGMTurnEnd::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bAutoActivateGameplayCue)
	{
		FGameplayCueParameters CueParams;
		// to do : init params
		K2_AddGameplayCueWithParams(KP_GameplayTags::GameplayCue_Basics_TurnEnd, CueParams);
	}


	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
