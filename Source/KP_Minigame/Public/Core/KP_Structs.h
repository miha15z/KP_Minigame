// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KP_Structs.generated.h"

class ACell;
class ABoardPiece;

USTRUCT()
struct FKPPawnData
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
struct FKPGenPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FKPPawnData> Pawns;

	UPROPERTY(EditInstanceOnly)
	FColor PlayerColor = FColor::Black;
};

USTRUCT(BlueprintType)
struct FKPPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	ABoardPiece* Pawn;
	UPROPERTY(BlueprintReadWrite)
	int32 CellId;
};

USTRUCT(BlueprintType)
struct FKPPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FKPPawnInfo> Pawns;
};

USTRUCT(BlueprintType)
struct FBoardData
{
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<ACell*> Cells;

	UPROPERTY(BlueprintReadWrite)
	TArray<FKPPlayerData> PlayersData;
};

USTRUCT(BlueprintType)
struct FBoardCoord
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

USTRUCT(BlueprintType)
struct FRollDicesData
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadOnly)
	int32 Value1;
	UPROPERTY(BlueprintReadOnly)
	int32 Value2;
};