// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/KP_Structs.h"
#include "GenDataAsset.generated.h"


class ACell;
/**
 * 
 */
UCLASS(BlueprintType)
class KP_MINIGAME_API UGenDataAsset : public UDataAsset
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "GameBoard|Navigation")
	TArray<FBoardCoord> MovementPattern = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	int32 BoardSize = 8;

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	TSubclassOf<ACell> CellClass;

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	FVector TopCellLocation;

	//  RightOffset = (LeftOffset.X, -LeftOffset.Y, LeftOffset.Z)
	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	FVector LeftOffset;

	// PlayersData[0] = player data, other = AI
	UPROPERTY(EditDefaultsOnly, Category = "GameBoard | Players")
	TArray<FKPGenPlayerData> PlayersData;  

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FBoardCoord> GetMovementPattern() const { return MovementPattern; }

	UFUNCTION(BlueprintCallable)
	const FKPGenPlayerData& GetPlayerData(const int32 PlayerId) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumPlayers() const {return PlayersData.Num(); }

	FORCEINLINE const TArray<FKPGenPlayerData>& GetAllPlayersData() const {return PlayersData;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetBoardSize() const {return BoardSize;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<ACell> GetCellClass() const { return CellClass; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetTopCellLocation() const { return TopCellLocation; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetCellLeftOffset() const { return LeftOffset; }
};
