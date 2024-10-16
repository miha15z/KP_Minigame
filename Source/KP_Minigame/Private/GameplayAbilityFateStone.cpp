// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityFateStone.h"


bool UGameplayAbilityFateStone::CanUseFateStone_Implementation(const AActor * Target, const AActor * Owner, int32 FateStoneActions) const 
{
    check(Target != nullptr && Owner != nullptr);

    return FateStoneActions > 0 ;
}

TArray<TSubclassOf<AActor>> UGameplayAbilityFateStone::GetTargetClasses() const 
{
    return TargetClasses;
}



