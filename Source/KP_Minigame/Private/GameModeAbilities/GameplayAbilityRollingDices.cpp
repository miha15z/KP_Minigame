// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityRollingDices.h"
#include "Core/KP_GameModeBase.h"
#include "KP_GameplayTags.h"
#include "KPPawn.h"

UGameplayAbilityGMRollingDices::UGameplayAbilityGMRollingDices(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_RollDices;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMRollingDices::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AKP_GameModeBase* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM && TriggerEventData && TriggerEventData->Instigator);
	GM->RollDices();
	Pawn = const_cast<AKPPawn*>(Cast<const AKPPawn>(TriggerEventData->Instigator));

	FGameplayCueParameters CueParams;
	// to do : init params
	K2_AddGameplayCueWithParams(KP_GameplayTags::GameplayCue_Basics_RollDices, CueParams);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityGMRollingDices::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	AKP_GameModeBase* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM && Pawn.IsValid());
	if (not bWasCancelled)
	{
		GM->OnRollDices();
		Pawn->UpdateRollDicesData();
		if (GM->IsBonusRollData())
		{
			//may be is not correction place for this call...
			GM->TryGiveBonus();
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

//bool UGameplayAbilityGMRollingDices::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
//{
//	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
//}

bool UGameplayAbilityGMRollingDices::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const
{
	if (Payload )
	{
		auto OwningActor = GetOwningActorFromActorInfo();
		AKP_GameModeBase* GM = Cast<AKP_GameModeBase>(OwningActor);
		check(GM);
		const AKPPawn* PawnPtr = Cast<const AKPPawn>(Payload->Instigator);
		return PawnPtr && GM->CanPlaerRollDices(PawnPtr);
	}
	return false;
}
