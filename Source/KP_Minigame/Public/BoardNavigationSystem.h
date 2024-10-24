// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/KP_Structs.h"
#include "BoardNavigationSystem.generated.h"

class ACell;
class ABoardPiece;
struct FBoardCoord;
/**
 * 
 */

// Class for storing minimal movements between nodes.
// UObject cause recursion/reference to the same type variables is not supported for UStruct.


UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API UBoardNavigationSystem : public UObject
{
	GENERATED_BODY()
	//TO DO: use shared or weak
	FBoardData* BoardDataRef = nullptr;

public:
	void Init(FBoardData* NewBoardData);

	UFUNCTION(BlueprintCallable)
	static void CalculateOrthogonalLength(const ACell * Origin, const ACell * Destination, float& Length);

	UFUNCTION(BlueprintCallable)
	static void CalculateNeighbouringCoordsByMask(const FBoardCoord& ReferenceCoord, const TArray<FBoardCoord>& MovementMask, TArray<FBoardCoord>& NeighbouringCoords);

	UFUNCTION(BlueprintCallable)
	static void SetupNeighbouringCellsByMask(const TArray<ACell*>& CellsOnBoard, const TArray<FBoardCoord>& MovementMask);
	
	UFUNCTION(BlueprintCallable)
	static void FindCellsByMask(const TArray<ACell*>& CellsOnBoard, const ACell* OriginCell, const TArray<FBoardCoord>& Mask, TArray<ACell*>& OutCells);

	UFUNCTION(BlueprintCallable)
	static void GetPossibleMovements(const TArray<ACell*>& CellsOnBoard, const ABoardPiece * OriginPiece, const ACell * OriginCell, const int32 MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements);

	UFUNCTION(BlueprintCallable)
	void GetPossibleMovementsLocalData(const ABoardPiece * OriginPiece, const int32 MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements);

	UFUNCTION(BlueprintCallable)
	static void GetMovementPathToCell(const TArray<FBoardAtomicMovement>& PossibleMovements, const ACell * OriginCell, const ACell * DestinationCell, bool& isPossible, TArray<FBoardAtomicMovement>& MovementPath);

	UFUNCTION(BlueprintCallable)
	static void CalculateAtomicMovement(const ACell * Origin, const ACell * const Destination, const ABoardPiece* const BoardPiece, const float MovementPoints, FBoardAtomicMovement& AtomicMovement);
};
