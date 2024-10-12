// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityFateStone.h"

UClass* UFateStoneData::GetGameplayAbilityClass() const
{
    return *GameplayAbility;
}

UTexture2D* UFateStoneData::GetIconTexture() const
{
    return IconUI;
}

// UMaterialInterface* UFateStoneData::GetCellMaterial() const
// {
//     return CellMaterial;
// }
