// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Generators/GameBoardGeneratorBase.h"
#include "Core/Generators/Data/GenDataAsset.h"

FBoardData UGameBoardGeneratorBase::GenerateGameBoard_Implementation(UGenDataAsset* GenData, UObject* WorldContext) const
{
	check(WorldContext);
	auto World = WorldContext->GetWorld();
	check(World && GenData);
	auto CellClass = GenData->GetCellClass();
	auto BoardSize = GenData->GetBoardSize();
	auto LeftOffset = GenData->GetCellLeftOffset();
	auto RightOffset = LeftOffset * FVector(1.f, -1.f, 1.f);
	auto TopCellLocation = GenData->GetTopCellLocation();

	return FBoardData();
}
