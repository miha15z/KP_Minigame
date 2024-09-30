// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Generators/GameBoardGeneratorBase.h"
#include "Core/Generators/Data/GenDataAsset.h"
#include "Cell.h"
#include "BoardPiece.h"

FBoardData UGameBoardGeneratorBase::GenerateGameBoard_Implementation(UGenDataAsset* GenData, UObject* WorldContext) const
{
	check(WorldContext);
	auto World = WorldContext->GetWorld();
	check(World && GenData);
	auto CellClass = GenData->GetCellClass().Get();
	check(CellClass);
	const int32 BoardSize = GenData->GetBoardSize();
	const FVector LeftOffset = GenData->GetCellLeftOffset();
	const FVector RightOffset = LeftOffset * FVector(1.f, -1.f, 1.f);
	const FVector TopCellLocation = GenData->GetTopCellLocation();

	FBoardData BoardData;
	// spawn cells
	for(int32 i = 0; i < BoardSize; ++i)
	{
		for (int32 j = 0; j < BoardSize; ++j)
		{
			auto CellActor = World->SpawnActor<ACell>(CellClass/*to do  spawn params*/);
			check(CellActor);
			CellActor->Coord = {i, j};
			CellActor->CellId = i * BoardSize + j;
			CellActor->SetActorLocation(TopCellLocation - (i * RightOffset + j * LeftOffset));
			BoardData.Cells.Add(CellActor);
		}
	}

	//apply cell's ability
	// to do

	// spawn pawns
	for (const auto& PlayerData : GenData->GetAllPlayersData())
	{
		for (const auto& PawnData : PlayerData.Pawns)
		{
			check(PawnData.PawnClass.Get());
			// to do
			//auto Pawn = World->SpawnActor<AKPPawnBase>(PawnData.PawnClass);
		}
	}

	return BoardData;
}

