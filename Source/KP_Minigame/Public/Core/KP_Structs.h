// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KP_Structs.generated.h"

class ACell;
class ABoardPiece;

USTRUCT()
struct KP_MINIGAME_API FKPPawnData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<ABoardPiece> PawnClass;

	UPROPERTY(EditInstanceOnly)
	int32 CellId = -1; // -1  =  not valid value!

	// to do: abilities for pawn
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FKPGenPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FKPPawnData> Pawns;

	UPROPERTY(EditInstanceOnly)
	FColor PlayerColor = FColor::Black;
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FKPPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	ABoardPiece* Pawn;
	UPROPERTY(BlueprintReadWrite)
	int32 CellId;
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FKPPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FKPPawnInfo> Pawns;
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FBoardData
{
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<ACell*> Cells;

	UPROPERTY(BlueprintReadWrite)
	TArray<FKPPlayerData> PlayersData;
};

USTRUCT(BlueprintType)
struct KP_MINIGAME_API FBoardCoord
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 y;

	bool operator==(FBoardCoord const& other) const {
		return (this->x == other.x) and (this->y == other.y);
	}
};

UENUM(BlueprintType)
enum class EBoardPiece : uint8
{
	None,
	Pawn,
	AdvancedPawn,
	King
};