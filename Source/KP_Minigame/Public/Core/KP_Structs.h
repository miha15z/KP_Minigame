// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KP_Structs.generated.h"

class ACell;
class AKPPawnBase;

USTRUCT()
struct KP_MINIGAME_API FKPPawnData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AKPPawnBase> PawnClass;

	UPROPERTY(EditInstanceOnly)
	int32 CellId = -1; // -1  =  not valid value!

	// to do: abilities for pawn
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FKPPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FKPPawnData> Pawns;

	UPROPERTY(EditInstanceOnly)
	FColor PlayerColor = FColor::Black;
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FBoardData
{
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<ACell*> Cells;

	//void operator=(FBoardData&& Other)
	//{
	//	Cells = MoveTemp(Other.Cells);
	//}
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FBoardCoord
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int y;

	bool operator==(FBoardCoord const& other) const {
		return (this->x == other.x) and (this->y == other.y);
	}
};
