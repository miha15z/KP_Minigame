// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNavigationSystem.h"
#include <Cell.h>

void UBoardNavigationSystem::CalculateOrthogonalLength(ACell* Origin, ACell* Destination, float& Length)
{
	Length = 0.f;//abs(Destination->CoordX - Origin->CoordX) - abs(Destination->CoordY - Origin->CoordY);
	return;
}

void UBoardNavigationSystem::CalculateNeighbouringCoordsByMask(FBoardCoord ReferenceCoord, TArray<FBoardCoord> MovementPatternMask, TArray<FBoardCoord>& NeighbouringCoords)
{
	TArray<FBoardCoord> ResultingArray;

	for (FBoardCoord Direction : MovementPatternMask) {
		FBoardCoord AffectedByMovement = ReferenceCoord;
		AffectedByMovement.x += Direction.x;
		AffectedByMovement.y += Direction.y;
		ResultingArray.Add(AffectedByMovement);
	}
	
	NeighbouringCoords = ResultingArray;
}
