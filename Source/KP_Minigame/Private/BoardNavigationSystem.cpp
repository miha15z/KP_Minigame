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

void UBoardNavigationSystem::CalculateOrthogonalLength(const ACell * Origin, const ACell * Destination, float& Length)
{
	Length = 0.f;//abs(Destination->CoordX - Origin->CoordX) - abs(Destination->CoordY - Origin->CoordY);
	return;
}

void UBoardNavigationSystem::CalculateNeighbouringCoordsByMask(const FBoardCoord& ReferenceCoord, const TArray<FBoardCoord>& MovementMask, TArray<FBoardCoord>& NeighbouringCoords)
{
	NeighbouringCoords.Empty();
	for (const FBoardCoord& Direction : MovementMask) 
	{
		FBoardCoord AffectedByMovement = ReferenceCoord;
		AffectedByMovement.x += Direction.x;
		AffectedByMovement.y += Direction.y;
		NeighbouringCoords.Add(AffectedByMovement);
	}
}

// TODO: Finish or remove
void UBoardNavigationSystem::SetupNeighbouringCellsByMask(const TArray<ACell*>& CellsOnBoard, const TArray<FBoardCoord>& MovementMask)
{
	const int32 CellNum = CellsOnBoard.Num();
	// For each cell on board
	for (ACell* OriginCell : CellsOnBoard) 
	{
		// Calculate coordinates for neighbouring cells
		TArray<FBoardCoord> NeighbouringCoords;
		UBoardNavigationSystem::CalculateNeighbouringCoordsByMask(OriginCell->Coord, MovementMask, NeighbouringCoords);
		// And find them among other cells on the board
		for (const FBoardCoord& NeighbouringCoord : NeighbouringCoords) 
		{
			for (int32 i = 0; (i < CellNum); i++) 
			{
				ACell* currentCell = CellsOnBoard[i];
				if (NeighbouringCoord == currentCell->Coord) 
				{
					OriginCell->Neighbours.Add(currentCell);
					break;
				}
			}
		}
	}

}

void UBoardNavigationSystem::FindCellsByMask(const TArray<ACell*>& CellsOnBoard, const ACell* OriginCell, const TArray<FBoardCoord>& Mask, TArray<ACell*>& OutCells)
{
	FBoardCoord OriginCoord = OriginCell->Coord;
	TArray<FBoardCoord> TargetCoords = Mask;
	for (auto& Coord : TargetCoords)
	{
		Coord = OriginCoord + Coord;
	}

	for (ACell* Cell : CellsOnBoard)
	{
		for (auto TargetCoord : TargetCoords)
		{
			if (TargetCoord == Cell->Coord)
			{
				OutCells.Add(Cell);
				break;
			}
		}
	}
}

void UBoardNavigationSystem::GetPossibleMovements(const TArray<ACell*>& CellsOnBoard, const ABoardPiece * OriginPiece, const ACell * OriginCell, const int32 MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements)
{
	// populate initial "open" array
	// in a while not empty loop over "open" queue:
	// add current movement to the "closed" array (limited by the action movement and maximum movement limit)
	// add movements to the neighbouring cells to the "open" array
	// end of loop
	// return the "closed" array

	PossibleMovements.Empty();

	TQueue<FBoardAtomicMovement> OpenQueue;

	// create movement info to the origin's neighbouring cells
	for (const auto& Neighbour : OriginCell->Neighbours) 
	{
		FBoardAtomicMovement ConstructedMovement;
		UBoardNavigationSystem::CalculateAtomicMovement(OriginCell, Neighbour.Get(), OriginPiece, (float)MovementPoints, ConstructedMovement);
		OpenQueue.Enqueue(ConstructedMovement);
	}
	while (not OpenQueue.IsEmpty()) 
	{
		FBoardAtomicMovement CurrentMovement;
		OpenQueue.Dequeue(CurrentMovement);

		// check the movement possibility

		if (CurrentMovement.MovementPointsLeft < -1*FLT_EPSILON) 
		{
			continue;
		}
		
		// find worse movements in the Closed Array and replace them
		bool isUnique = true;
		for (int32 i = 0; i < PossibleMovements.Num(); i++)
		{
			FBoardAtomicMovement ClosedMovement = PossibleMovements[i];
			if (ClosedMovement.CellTo->Coord == CurrentMovement.CellTo->Coord)
			{
				isUnique = false;
				if (ClosedMovement.MovementPointsLeft < CurrentMovement.MovementPointsLeft) 
				{
					PossibleMovements[i] = CurrentMovement;
				}
			}
		}
		
		// if no same-destination movements found, then add the new one
		if (isUnique)
		{
			PossibleMovements.Add(CurrentMovement);
		}
		
		// add movements to the neighbouring cells to the "open" array
		for (const auto& Neighbour : CurrentMovement.CellTo->Neighbours) 
		{
			FBoardAtomicMovement ConstructedMovement;
			CalculateAtomicMovement(CurrentMovement.CellTo.Get(), Neighbour.Get(), OriginPiece, (float)CurrentMovement.MovementPointsLeft, ConstructedMovement);
			OpenQueue.Enqueue(ConstructedMovement);
		}
	}
	
}

void UBoardNavigationSystem::GetPossibleMovementsLocalData(const ABoardPiece * OriginPiece, const int32 MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements)
{
	GetPossibleMovements(BoardDataRef->Cells, OriginPiece, BoardDataRef->GetGellByIdChecked(OriginPiece->GetCurrentCellId()), MovementPoints, PossibleMovements);
}

void UBoardNavigationSystem::GetMovementPathToCell(const TArray<FBoardAtomicMovement>& PossibleMovements, const ACell * OriginCell, const ACell * DestinationCell, bool& isPossible, TArray<FBoardAtomicMovement>& MovementPath)
{
	// check if origin cell is there
	bool isOriginCellThere = false;
	for (const FBoardAtomicMovement& Movement : PossibleMovements) 
	{
		if (Movement.CellFrom == OriginCell)
		{
			isOriginCellThere = true;
			break;
		}
	}
	if (not isOriginCellThere)
	{
		isPossible = false;
		return;
	}
	else
	{
		isPossible = true;
	}

	MovementPath.Empty();
	// find the destination movement
	FBoardAtomicMovement FirstBackTrackMovement;
	bool isThereFirstBackTrackMovement = false;
	for (const FBoardAtomicMovement& Movement : PossibleMovements) 
	{
		if (Movement.CellTo == DestinationCell) 
		{
			FirstBackTrackMovement = Movement;
			isThereFirstBackTrackMovement = true;
			break;
		}
	}
	if (not isThereFirstBackTrackMovement) 
	{
		isPossible = false;
		return;
	}

	// find the successive back track movements until OriginCell
	const ACell* CurrentCellFrom = FirstBackTrackMovement.CellFrom.Get();
	MovementPath.Add(FirstBackTrackMovement);

	while (CurrentCellFrom != OriginCell)
	{
		for (const FBoardAtomicMovement& Movement : PossibleMovements) 
		{
			if (Movement.CellTo == CurrentCellFrom) 
			{
				CurrentCellFrom = Movement.CellFrom.Get();
				MovementPath.Add(Movement);
				break;
			}
		}
	}

	isPossible = true;
	return;
}

void UBoardNavigationSystem::CalculateAtomicMovement(const ACell * Origin, const ACell * const Destination, const ABoardPiece* const  BoardPiece, const float MovementPoints, FBoardAtomicMovement& OutAtomicMovement)
{
	OutAtomicMovement.CellTo = const_cast<ACell*>(Destination);
	OutAtomicMovement.CellFrom = const_cast<ACell*>(Origin);
	
	const FBoardCoord PendingMovementDirection = Destination->Coord - Origin->Coord;
	const TArray<FBoardCoord>& PossibleDirections = BoardPiece->GetMovementDirections();
	
	// Check if this direction is allowed for the pawn
	bool bMovementDirectionAllowed = false;
	for (const FBoardCoord& PossibleDirection : PossibleDirections) 
	{
		if (PossibleDirection == PendingMovementDirection) 
		{
			bMovementDirectionAllowed = true;
		}
	}

	// If the direction is not allowed, then cast the movement impossible
	if (not bMovementDirectionAllowed)
	{
		OutAtomicMovement.MovementPointsLeft = -1.0;
		OutAtomicMovement.MovementPointsConsumed = MovementPoints - OutAtomicMovement.MovementPointsLeft;
		return;
	}

	// Check direction safety for the king
	const TArray<TWeakObjectPtr<ACell> >& DestinationNeighbours = Destination->Neighbours;
	for (const auto& NeighbouringCell : DestinationNeighbours)
	{
		const ABoardPiece* Pawn = NeighbouringCell->GetStoodPawn();
		if (not Pawn)
		{
			continue;
		}
		const FBoardCoord PendingMovement = Destination->Coord - NeighbouringCell->Coord;
		bool bMovementPossible = false;
		for (const auto& PossibleMovement : Pawn->GetMovementDirections())
		{
			if (PossibleMovement == PendingMovement)
			{
				bMovementPossible = true;
			}
		}
		if (Pawn->GetOwnPlayerId() != BoardPiece->GetOwnPlayerId() && bMovementPossible)
		{
			OutAtomicMovement.bSafeForKing = false;
		}
	}

	// Trim available movement points
	const float MaxMovementPointsOverride = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMaxMovementPointsOverrideAttribute());
	const float AvailableMovementPoints =  (MaxMovementPointsOverride < -DBL_EPSILON) ? MovementPoints : FMath::Min(MaxMovementPointsOverride, MovementPoints);

	// Check if there is any BoardPiece in the destination.
	const ABoardPiece* DestinationBoardPiece = Destination->GetStoodPawn();
	if (DestinationBoardPiece != NULL) 
	{
		// If there is a board piece of the same team, do not allow to enter this cell
		if (BoardPiece->GetOwnPlayerId() == DestinationBoardPiece->GetOwnPlayerId()) 
		{
			OutAtomicMovement.MovementPointsLeft = -1.0;
		}
		// If there is an enemy pawn, make that the final movement
		else 
		{
			const float MovementOut = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementOutAttribute());
			const float MovementIn = Destination->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementInAttribute());
			const float BoardPieceMovementPointsModifier = BoardPiece->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsModifierAttribute());
			// calculate movement points amount that is to be consumed
			const float MovementPointsConsumed = FMath::Max(MovementIn, MovementOut) * BoardPieceMovementPointsModifier;
			// If there are any points left after this movement, dispose of them
			const float MovementPointsLeft = FMath::Min(AvailableMovementPoints - MovementPointsConsumed, 0.0);
			OutAtomicMovement.MovementPointsLeft = MovementPointsLeft;
		}
		OutAtomicMovement.MovementPointsConsumed = MovementPoints - OutAtomicMovement.MovementPointsLeft;
	}
	// If the cell is in the clear
	else 
	{
		const float MovementOut = Origin->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementOutAttribute());
		const float MovementIn = Destination->GetAbilitySystemComponent()->GetNumericAttribute(UCellAttributeSet::GetMovementInAttribute());
		const float BoardPieceMovementPointsModifier = BoardPiece->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsModifierAttribute());
		const float MovementPointsConsumed = FMath::Max(MovementIn, MovementOut) * BoardPieceMovementPointsModifier;
		const float MovementPointsLeft = AvailableMovementPoints - MovementPointsConsumed;
		OutAtomicMovement.MovementPointsLeft = MovementPointsLeft;
		OutAtomicMovement.MovementPointsConsumed = MovementPoints - OutAtomicMovement.MovementPointsLeft;
	}
}
