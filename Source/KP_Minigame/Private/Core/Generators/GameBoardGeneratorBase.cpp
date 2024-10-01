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

	// Setup Special Cell cases
	for (FKPCellData CellData : GenData->GetCellsData()) {
		for (ACell* Cell : BoardData.Cells) {
			if (CellData.CellId == Cell->CellId) {
				// Setup cell abilities
				Cell->StartupAbilities = CellData.StartupAbilities;
			}
		}
	}

	// spawn pawns
	int32 CurrentPlayerId = 0;
	for (const auto& PlayerData : GenData->GetAllPlayersData())
	{
		FKPPlayerData PlayerPawnsData;
		for (const auto& PawnData : PlayerData.Pawns)
		{
			check(PawnData.PawnClass.Get());
			// to do
			auto Pawn = World->SpawnActor<ABoardPiece>(PawnData.PawnClass);
			Pawn->SetNewCellId(PawnData.CellId);
			// Use Array.FindByPredicate or BinareSerch
			if (PawnData.CellId >= 0 && PawnData.CellId < BoardData.Cells.Num())
			{
				ACell* Cell = BoardData.Cells[PawnData.CellId];
				Pawn->SetActorLocation(Cell->GetActorLocation());
				PlayerPawnsData.Pawns.Add({Pawn ,PawnData.CellId });
				Cell->StoodPawnOnCell(Pawn);
				Pawn->SetOwnPlayerData(CurrentPlayerId, PlayerData.PlayerColor);
			}
			else
			{
				check(false);
			}
			
		}
		BoardData.PlayersData.Add(MoveTemp(PlayerPawnsData));
		++CurrentPlayerId;
	}

	return BoardData;
}

