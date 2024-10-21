// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityFateStone.h"
#include "KPPawn.h"


bool UGameplayAbilityFateStone::CanUseFateStone_Implementation(const AActor * Target, const AActor * Owner, int32 FateStoneActions) const 
{
    check(Target != nullptr && Owner != nullptr);

    return FateStoneActions > 0 ;
}

TArray<TSubclassOf<AActor>> UGameplayAbilityFateStone::GetTargetClasses() const 
{
    return TargetClasses;
}

void UGameplayAbilityFateStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (TriggerEventData)
    {
        Instigator = const_cast<AKPPawn*>(Cast<const AKPPawn>(TriggerEventData->Instigator));
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityFateStone::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (not bWasCancelled && Instigator.IsValid())
    {
        Instigator->EndUseFateStone();
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



