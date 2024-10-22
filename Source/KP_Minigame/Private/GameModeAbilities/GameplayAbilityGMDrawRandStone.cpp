// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMDrawRandStone.h"
#include "KPPawn.h"
#include "Core/KP_GameModeBase.h"
#include "KP_GameplayTags.h"

UGameplayAbilityGMDrawRandStone::UGameplayAbilityGMDrawRandStone(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_SelectRandomFateStone;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMDrawRandStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData);
	Pawn = const_cast<AKPPawn*>(Cast<const AKPPawn>(TriggerEventData->Target));
	check(Pawn.IsValid());
	auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM);
	IndexStone = GM->GetRandomFateStoneIndex();
	SelectedFateStoneData = GM->GetFateStoneData(IndexStone);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityGMDrawRandStone::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (not bWasCancelled && Pawn.IsValid())
	{
		auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
		check(GM);
		bWasCancelled = GM->TryCurrentPawnGiveFateStone(IndexStone);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbilityGMDrawRandStone::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const
{
	check(Payload);
	auto tempPawn = const_cast<AKPPawn*>(Cast<const AKPPawn>(Payload->Target));
	check(tempPawn);
	auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM);
	bool bCanUse = GM->IsDrawingRandomFateStone() && GM->CanGiveFateStone(tempPawn) && tempPawn->CanGiveFateStone();
	return bCanUse && Super::ShouldAbilityRespondToEvent(ActorInfo, Payload);
	//return false;
}
