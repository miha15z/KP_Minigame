// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnAbilities/GameplayAbilityPPSelectFateStone.h"
#include "KP_GameplayTags.h"
#include "KPPawn.h"
#include "SelectFateStoneHolder.h"

UGameplayAbilityPPSelectFateStone::UGameplayAbilityPPSelectFateStone(const FObjectInitializer& ObjectInitializer)
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

void UGameplayAbilityPPSelectFateStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData && TriggerEventData->OptionalObject);
	auto* Pawn = Cast<AKPPawn>(GetOwningActorFromActorInfo());
	check(Pawn);
	auto FateStoneDataHolder = Cast<USelectFateStoneHolder>(TriggerEventData->OptionalObject);
	check(FateStoneDataHolder);
	Pawn->SelectFateStone(FateStoneDataHolder->FateStoneIndex);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
