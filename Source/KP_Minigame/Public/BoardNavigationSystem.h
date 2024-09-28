// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNavigationSystem.generated.h"

class ACell;
struct FBoardCoord;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API UBoardNavigationSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CalculateOrthogonalLength(ACell* Origin, ACell* Destination, float& Length);

	UFUNCTION(BlueprintCallable)
	void CalculateNeighbouringCoordsByMask(FBoardCoord ReferenceCoord, TArray<FBoardCoord> MovementPatternMask, TArray<FBoardCoord>& NeighbouringCoords);
};
