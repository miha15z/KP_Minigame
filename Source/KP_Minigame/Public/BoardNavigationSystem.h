// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNavigationSystem.generated.h"

class ACell;
// TODO: Create actual class
class ABoardPiece;
struct FBoardCoord;
/**
 * 
 */

// Class for storing minimal movements between nodes.
// UObject cause recursion/reference to the same type variables is not supported for UStruct.
USTRUCT(BlueprintType, Blueprintable)
struct FBoardAtomicMovement{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	ACell* CellTo;
	UPROPERTY()
	ACell* CellFrom;
	UPROPERTY()
	float MovementPointsLeft;
	UPROPERTY()
	float MovementPointsConsumed;
};

UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API UBoardNavigationSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CalculateOrthogonalLength(ACell* Origin, ACell* Destination, float& Length);

	UFUNCTION(BlueprintCallable)
	void CalculateNeighbouringCoordsByMask(FBoardCoord ReferenceCoord, TArray<FBoardCoord> MovementMask, TArray<FBoardCoord>& NeighbouringCoords);

	UFUNCTION(BlueprintCallable)
	void SetupNeighbouringCellsByMask(TArray<ACell*> CellsOnBoard, TArray<FBoardCoord> MovementMask);
	
	UFUNCTION(BlueprintCallable)
	void GetPossibleMovements(TArray<ACell*> CellsOnBoard, ABoardPiece* OriginPiece, ACell* OriginCell, int MovementPoints, TArray<FBoardAtomicMovement>& PossibleMovements);

	UFUNCTION(BlueprintCallable)
	void CalculateAtomicMovement(ACell* Origin, ACell* Destination, ABoardPiece* BoardPiece, float MovementPoints, FBoardAtomicMovement& AtomicMovement);
};
