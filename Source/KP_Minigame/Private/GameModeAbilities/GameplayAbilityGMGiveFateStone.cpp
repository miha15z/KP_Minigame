// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMGiveFateStone.h"
#include "KP_GameplayTags.h"
#include "Core/KP_GameModeBase.h"
#include "SelectFateStoneHolder.h"

UGameplayAbilityGMGiveFateStone::UGameplayAbilityGMGiveFateStone(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_SelectFateStone;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMGiveFateStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData && TriggerEventData->OptionalObject);
	auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM);
	auto FateStoneDataHolder = Cast<USelectFateStoneHolder>(TriggerEventData->OptionalObject);
	check(FateStoneDataHolder);
	if (not GM->TryCurrentPawnGiveFateStone(FateStoneDataHolder->FateStoneIndex))
	{
		const bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, false, bWasCancelled);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
