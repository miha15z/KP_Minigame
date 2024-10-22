// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMTurnEnd.h"
#include "KP_GameplayTags.h"
#include "Core/KP_GameModeBase.h"
#include "KPPawn.h"

UGameplayAbilityGMTurnEnd::UGameplayAbilityGMTurnEnd(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_EndTurn;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
		this->AbilityTags.AddTag(KP_GameplayTags::Gameplay_Ability_EndTurn);
	}
}

void UGameplayAbilityGMTurnEnd::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bAutoActivateGameplayCue)
	{
		FGameplayCueParameters CueParams;
		// to do : init params
		K2_AddGameplayCueWithParams(KP_GameplayTags::GameplayCue_Basics_TurnEnd, CueParams);
	}
	check(TriggerEventData && TriggerEventData->Instigator);
	PlayerPawn = const_cast<AKPPawn*>(Cast<const AKPPawn>(TriggerEventData->Instigator));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityGMTurnEnd::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (not bWasCancelled && PlayerPawn.IsValid())
	{
		auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
		check(GM);
		GM->EndTurn(PlayerPawn.Get());
		PlayerPawn = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbilityGMTurnEnd::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const
{
	if (Payload)
	{
		//to do:   check CanPlaerEndTurn 
		return true && Super::ShouldAbilityRespondToEvent(ActorInfo, Payload);
	}
	return false;
}
