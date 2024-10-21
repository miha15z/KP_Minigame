// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMPlaceStoneInBag.h"
#include "KP_GameplayTags.h"

UGameplayAbilityGMPlaceStoneInBag::UGameplayAbilityGMPlaceStoneInBag(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_PlaceStoneInBag;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMPlaceStoneInBag::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bAutoActivateGameplayCue)
	{
		FGameplayCueParameters CueParams;
		// to do : init params
		K2_AddGameplayCueWithParams(KP_GameplayTags::GameplayCue_FateStone_PlaceInBag, CueParams);
	}

	//TriggerEventData->Instigator - AKPPawn;
	//TriggerEventData->OptionalObject - UFateStoneDataAsset


	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
