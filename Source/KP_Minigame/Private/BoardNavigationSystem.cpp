// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNavigationSystem.h"
#include <Cell.h>
#include <CellAttributeSet.h>
#include <BoardPiece.h>
#include <BoardPieceAttributeSet.h>
#include "Kismet/KismetMathLibrary.h"
#include "Core/KP_Structs.h"
#include "AbilitySystemComponent.h"

void UBoardNavigationSystem::CalculateOrthogonalLength(ACell* Origin, ACell* Destination, float& Length)
{
	Length = 0.f;//abs(Destination->CoordX - Origin->CoordX) - abs(Destination->CoordY - Origin->CoordY);
	return;
}

void UBoardNavigationSystem::CalculateNeighbouringCoordsByMask(FBoardCoord ReferenceCoord, TArray<FBoardCoord> MovementMask, TArray<FBoardCoord>& NeighbouringCoords)
{
	TArray<FBoardCoord> ResultingArray;

	for (FBoardCoord Direction : MovementMask) {
		FBoardCoord AffectedByMovement = ReferenceCoord;
		AffectedByMovement.x += Direction.x;
		AffectedByMovement.y += Direction.y;
		ResultingArray.Add(AffectedByMovement);
	}
	
	NeighbouringCoords = ResultingArray;
}

// TODO: Finish or remove
void UBoardNavigationSystem::SetupNeighbouringCellsByMask(TArray<ACell*> CellsOnBoard, TArray<FBoardCoord> MovementMask)
{
	int CellNum = CellsOnBoard.Num();
	// For each cell on board
	for (ACell* OriginCell : CellsOnBoard) {
		// Calculate coordinates for neighbouring cells
		TArray<FBoardCoord> NeighbouringCoords;
		UBoardNavigationSystem::CalculateNeighbouringCoordsByMask(OriginCell->Coord, MovementMask, NeighbouringCoords);
		// And find them among other cells on the board
		for (FBoardCoord NeighbouringCoord : NeighbouringCoords) {
			bool isFound = false;
			for (int i = 0; (i < CellNum); i++) {
				ACell* currentCell = CellsOnBoard[i];
				if (NeighbouringCoord == currentCell->Coord) {
					OriginCell->Neighbours.Add(currentCell);
					break;
				}
			}
		}
	}

}

void UBoardNavigationSystem::GetPossibleMovements(TArray<ACell*> CellsOnBoard, ABoardPiece* OriginPiece, ACell* OriginCell, int MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements)
{
	// populate initial "open" array
	// in a while not empty loop over "open" queue:
	// add current movement to the "closed" array (limited by the action movement and maximum movement limit)
	// add movements to the neighbouring cells to the "open" array
	// end of loop
	// return the "closed" array

	TQueue<FBoardAtomicMovement> OpenQueue;
	// WARNING: Might be succeptible for wrongful garbage collection
	//TArray<UBoardAtomicMovement*> ClosedArray;

	// create movement info to the origin's neighbouring cells
	for (ACell* Neighbour : OriginCell->Neighbours) {
		FBoardAtomicMovement ConstructedMovement;
		UBoardNavigationSystem::CalculateAtomicMovement(OriginCell, Neighbour, OriginPiece, (float) MovementPoints, ConstructedMovement);
		OpenQueue.Enqueue(ConstructedMovement);
	}
	while (not OpenQueue.IsEmpty()) {
		FBoardAtomicMovement CurrentMovement;
		OpenQueue.Dequeue(CurrentMovement);

		// check the movement possibility

		if (CurrentMovement.MovementPointsLeft < -1*FLT_EPSILON) {
			continue;
		}
		
		// find worse movements in the Closed Array and replace them
		bool isUnique = true;
		for (int i = 0; i < PossibleMovements.Num(); i++){
			FBoardAtomicMovement ClosedMovement = PossibleMovements[i];
			if (ClosedMovement.CellTo->Coord == CurrentMovement.CellTo->Coord){
				isUnique = false;
				if (ClosedMovement.MovementPointsLeft < CurrentMovement.MovementPointsLeft) {
					PossibleMovements[i] = CurrentMovement;
				}
			}
		}
		
		// if no same-destination movements found, then add the new one
		if (isUnique) {
			PossibleMovements.Add(CurrentMovement);
		}
		
		// add movements to the neighbouring cells to the "open" array
		for (ACell* Neighbour : CurrentMovement.CellTo->Neighbours) {
			FBoardAtomicMovement ConstructedMovement;
			CalculateAtomicMovement(OriginCell, Neighbour, OriginPiece, (float)CurrentMovement.MovementPointsLeft, ConstructedMovement);
			OpenQueue.Enqueue(ConstructedMovement);
		}
	}
	
}

void UBoardNavigationSystem::CalculateAtomicMovement(ACell* Origin, ACell* Destination, ABoardPiece* BoardPiece, float MovementPoints, FBoardAtomicMovement& AtomicMovement)
{
	AtomicMovement.CellTo = Destination;
	AtomicMovement.CellFrom = Origin;
	
	const float MovementOut = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementOutAttribute());
	const float MovementIn = Destination->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementInAttribute());
	const float BoardPieceMovementPointsModifier = BoardPiece->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsModifierAttribute());
	AtomicMovement.MovementPointsConsumed = MovementIn; //* BoardPieceMovementPointsModifier;
	AtomicMovement.MovementPointsLeft = MovementPoints - AtomicMovement.MovementPointsConsumed;
}
