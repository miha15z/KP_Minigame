// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNavigationSystem.h"
#include <Cell.h>
#include <CellAttributeSet.h>
#include <BoardPiece.h>
#include <BoardPieceAttributeSet.h>
#include "Kismet/KismetMathLibrary.h"
#include "Core/KP_Structs.h"
#include "AbilitySystemComponent.h"

void UBoardNavigationSystem::Init(FBoardData* NewBoardData)
{
	BoardDataRef = NewBoardData;
}

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
void UBoardNavigationSystem::SetupNeighbouringCellsByMask(TArray<ACell*>& CellsOnBoard, const TArray<FBoardCoord>& MovementMask)
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

void UBoardNavigationSystem::GetPossibleMovements(const TArray<ACell*>& CellsOnBoard, ABoardPiece* OriginPiece, ACell* OriginCell, int MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements)
{
	// populate initial "open" array
	// in a while not empty loop over "open" queue:
	// add current movement to the "closed" array (limited by the action movement and maximum movement limit)
	// add movements to the neighbouring cells to the "open" array
	// end of loop
	// return the "closed" array

	PossibleMovements.Empty();

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
			CalculateAtomicMovement(CurrentMovement.CellTo, Neighbour, OriginPiece, (float)CurrentMovement.MovementPointsLeft, ConstructedMovement);
			OpenQueue.Enqueue(ConstructedMovement);
		}
	}
	
}

void UBoardNavigationSystem::GetPossibleMovementsLocalData(ABoardPiece* OriginPiece, int MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements)
{
	GetPossibleMovements(BoardDataRef->Cells, OriginPiece, BoardDataRef->Cells[OriginPiece->GetCurrentCellId()], MovementPoints, PossibleMovements);
}

void UBoardNavigationSystem::GetMovementPathToCell(const TArray<FBoardAtomicMovement>& PossibleMovements, ACell* OriginCell, ACell* DestinationCell, bool& isPossible, TArray<FBoardAtomicMovement>& MovementPath)
{
	// check if origin cell is there
	bool isOriginCellThere = false;
	for (FBoardAtomicMovement Movement : PossibleMovements) {
		if (Movement.CellFrom == OriginCell) {
			isOriginCellThere = true;
			break;
		}
	}
	if (not isOriginCellThere) {
		isPossible = false;
		return;
	}
	else {
		isPossible = true;
	}

	MovementPath.Empty();
	// find the destination movement
	FBoardAtomicMovement FirstBackTrackMovement;
	bool isThereFirstBackTrackMovement = false;
	for (FBoardAtomicMovement Movement : PossibleMovements) {
		if (Movement.CellTo == DestinationCell) {
			FirstBackTrackMovement = Movement;
			isThereFirstBackTrackMovement = true;
			break;
		}
	}
	if (not isThereFirstBackTrackMovement) {
		isPossible = false;
		return;
	}

	// find the successive back track movements until OriginCell
	FBoardAtomicMovement CurrentBackTrackMovement = FirstBackTrackMovement;
	ACell* CurrentCellFrom = CurrentBackTrackMovement.CellFrom;
	MovementPath.Add(CurrentBackTrackMovement);

	while (CurrentCellFrom != OriginCell) {
		for (FBoardAtomicMovement Movement : PossibleMovements) {
			if (Movement.CellTo == CurrentCellFrom) {
				CurrentBackTrackMovement = Movement;
				CurrentCellFrom = CurrentBackTrackMovement.CellFrom;
				MovementPath.Add(CurrentBackTrackMovement);
				break;
			}
		}
	}

	isPossible = true;
	return;
}

void UBoardNavigationSystem::CalculateAtomicMovement(ACell* Origin, ACell* Destination, ABoardPiece* BoardPiece, float MovementPoints, FBoardAtomicMovement& AtomicMovement)
{
	AtomicMovement.CellTo = Destination;
	AtomicMovement.CellFrom = Origin;
	

	// Check if there is any BoardPiece in the destination.
	ABoardPiece* DestinationBoardPiece = Destination->GetStoodPawn();
	if (DestinationBoardPiece != NULL) {
		// If there is a board piece of the same team, do not allow to enter this cell
		if (BoardPiece->GetOwnPlayerId() == DestinationBoardPiece->GetOwnPlayerId()) {
			AtomicMovement.MovementPointsLeft = -1.0;
		}
		// If there is an enemy pawn, make that the final movement
		else {
			const float MovementOut = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementOutAttribute());
			const float MovementIn = Destination->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementInAttribute());
			const float BoardPieceMovementPointsModifier = BoardPiece->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsModifierAttribute());
			// calculate movement points amount that is to be consumed
			float MovementPointsConsumed = FMath::Max(MovementIn, MovementOut) * BoardPieceMovementPointsModifier;
			// If there are any points left after this movement, dispose of them
			float MovementPointsLeft = FMath::Min(MovementPointsConsumed, 0.0);
			AtomicMovement.MovementPointsLeft = MovementPointsLeft;
		}
		AtomicMovement.MovementPointsConsumed = MovementPoints - AtomicMovement.MovementPointsLeft;
	}
	// If the cell is in the clear
	else {
		const float MovementOut = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementOutAttribute());
		const float MovementIn = Destination->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementInAttribute());
		const float BoardPieceMovementPointsModifier = BoardPiece->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsModifierAttribute());
		AtomicMovement.MovementPointsConsumed = FMath::Max(MovementIn, MovementOut) * BoardPieceMovementPointsModifier;
		AtomicMovement.MovementPointsLeft = MovementPoints - AtomicMovement.MovementPointsConsumed;
	}
}
