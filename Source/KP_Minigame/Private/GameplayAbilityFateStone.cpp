// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityFateStone.h"


bool UGameplayAbilityFateStone::CanUseFateStone_Implementation(AActor* Target, AActor* Owner, int32 FateStoneActions)
{
    check(Target != nullptr && Owner != nullptr);

    return FateStoneActions > 0 ;
}

TArray<TSubclassOf<AActor>> UGameplayAbilityFateStone::GetTargetClasses()
{
    return TargetClasses;
}


UClass* UFateStoneData::GetGameplayAbilityClass() const
{
    return GameplayAbility.LoadSynchronous();
}

UTexture2D* UFateStoneData::GetIconTexture() const
{
    return IconUI.LoadSynchronous();
}

// UMaterialInterface* UFateStoneData::GetCellMaterial() const
// {
//     return CellMaterial;
// }


