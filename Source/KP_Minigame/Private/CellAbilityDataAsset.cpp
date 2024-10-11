// Fill out your copyright notice in the Description page of Project Settings.


#include "CellAbilityDataAsset.h"
#include "GameplayAbilityCellToPawnBase.h"

UClass* UCellAbilityDataAsset::GetAbilityClass() const
{
    return *AbilityClass;
}

UMaterialInterface* UCellAbilityDataAsset::GetCellMaterial() const
{
    return CellMaterial.LoadSynchronous();
}
