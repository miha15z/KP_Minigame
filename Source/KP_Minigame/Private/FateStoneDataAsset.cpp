// Fill out your copyright notice in the Description page of Project Settings.


#include "FateStoneDataAsset.h"
#include "GameplayAbilityFateStone.h"

UClass* UFateStoneDataAsset::GetGameplayAbilityClass() const
{
	return GameplayAbility.LoadSynchronous();
}

UTexture2D* UFateStoneDataAsset::GetIconTexture() const
{
	return IconUI.LoadSynchronous();
}

const FText& UFateStoneDataAsset::GetName() const
{
	return Name;
}

const FText& UFateStoneDataAsset::GetDescrption() const
{
	return Description;
}
