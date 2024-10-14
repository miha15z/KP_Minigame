// Fill out your copyright notice in the Description page of Project Settings.


#include "CellAbilityDataAsset.h"
#include "GameplayAbilityCellToPawnBase.h"

UClass* UCellAbilityDataAsset::GetAbilityClass() const
{
    return AbilityClass.LoadSynchronous();
}

UMaterialInterface* UCellAbilityDataAsset::GetCellMaterial() const
{
    return CellMaterial.LoadSynchronous();
}
