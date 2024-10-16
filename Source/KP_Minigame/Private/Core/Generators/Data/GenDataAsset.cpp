// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Generators/Data/GenDataAsset.h"
#include "Cell.h"



const FKPGenPlayerData& UGenDataAsset::GetPlayerData(const int32 PlayerId) const
{
	check(PlayerId < PlayersData.Num() && PlayerId >= 0);
	return PlayersData[PlayerId];
}

UClass* UGenDataAsset::GetCellClass() const
{
	return CellClass.LoadSynchronous();
}
