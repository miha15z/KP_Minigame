// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Generators/Data/GenDataAsset.h"



const FKPPlayerData& UGenDataAsset::GetPlayerData(const int32 PlayerId) const
{
	check(PlayerId < PlayersData.Num() && PlayerId >= 0);
	return PlayersData[PlayerId];
}
