// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPieceAbilities/GA_MoveToCell.h"
#include "AbilitySystemComponent.h"
#include "KP_GameplayTags.h"

UGameplayAbility_MoveToCell::UGameplayAbility_MoveToCell(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_MoveBoardPiece;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbility_MoveToCell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbilityFromEvent(*TriggerEventData);
	}
	else if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		//UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use 'Activate Ability' instead of 'Activate Ability From Event' in the Blueprint."), *GetName());
		constexpr bool bReplicateEndAbility = false;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	else
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			constexpr bool bReplicateEndAbility = true;
			constexpr bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}

		if (IsValid(TriggerEventData->Target))
		{
			auto OwningActor = GetOwningActorFromActorInfo();

			auto* GASC = GetAbilitySystemComponentFromActorInfo();
			GASC->SetLooseGameplayTagCount(KP_GameplayTags::BoardPieceState_Move, 1);
			//to do Apply a GameplayEffect or a gameplayCue
			//GASC->ApplyGameplayEffectToSelf()
			
			// to do  make Async Task for "Move"
			OwningActor->SetActorLocation(TriggerEventData->Target->GetActorLocation());
		}


		const bool bReplicateEndAbility = false;
		const bool bWasCanceled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
	}

	// not use this!!!
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_MoveToCell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	auto* GASC = GetAbilitySystemComponentFromActorInfo();
	GASC->SetLooseGameplayTagCount(KP_GameplayTags::BoardPieceState_Move, 0);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
