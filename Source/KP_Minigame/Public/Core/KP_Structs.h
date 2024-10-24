// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "KP_Structs.generated.h"

class ACell;
class ABoardPiece;
class UGameplayAbilityCellToPawnBase;
class UCellAbilityDataAsset;
class UFateStoneDataAsset;

USTRUCT(BlueprintType)
struct FBoardCoord
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 y;

	FBoardCoord operator*(FBoardCoord const& other) const 
	{
		return FBoardCoord{ (this->x * other.x), (this->y * other.y) };
	}

	FBoardCoord operator-(FBoardCoord const& other) const 
	{
		return FBoardCoord{ (this->x - other.x), (this->y - other.y) };
	}

	FBoardCoord operator+(FBoardCoord const& other) const
	{
		return FBoardCoord{ (this->x + other.x), (this->y + other.y) };
	}

	bool operator==(FBoardCoord const& other) const 
	{
		return (this->x == other.x) and (this->y == other.y);
	}
};

USTRUCT()
struct FKPPawnData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TSoftClassPtr<ABoardPiece> PawnClass;

	UPROPERTY(EditInstanceOnly)
	int32 CellId = -1; // -1  =  not valid value!
	// to do: abilities for pawn
};

// Structure for storing UGameplayAbilityCellToPawnBase Information both for setup and ability activation
USTRUCT(BlueprintType)
struct FGameplayAbilityCellToPawnInfo {
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TSoftObjectPtr<UCellAbilityDataAsset> Data;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 OwningPlayerID = -1;;
};

USTRUCT(BlueprintType)
struct FKPCellData {
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
	TArray<FGameplayAbilityCellToPawnInfo> StartupAbilitiesInfo;

	UPROPERTY(EditInstanceOnly)
	int32 CellId = -1;
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

	// Direction multiplier for ABoardPiece.MovementDirections
	// Use for changing team movement on the board
	UPROPERTY(EditInstanceOnly)
	FBoardCoord PawnsDirectionMultiplier;

	// may be only bots
	UPROPERTY(EditInstanceOnly)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > StartupFateStones;
};

USTRUCT(BlueprintType)
struct FKPPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ABoardPiece> Pawn;
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

	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UFateStoneDataAsset> > FateStonesInGame;
};

USTRUCT(BlueprintType)
struct FBoardData
{
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<ACell> > Cells;

	UPROPERTY(BlueprintReadWrite)
	TArray<FKPPlayerData> PlayersData;

	ACell* GetGellByIdChecked(const int32 Id) const;
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
	int32 Value1 =-1; // Value1 != Value2
	UPROPERTY(BlueprintReadOnly)
	int32 Value2 = 0; // Value1 != Value2
};

USTRUCT(BlueprintType, Blueprintable)
struct FBoardAtomicMovement {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACell> CellTo = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACell> CellFrom = nullptr;
	UPROPERTY()
	float MovementPointsLeft;
	UPROPERTY()
	float MovementPointsConsumed;
	UPROPERTY(BlueprintReadOnly)
	bool bSafeForKing = true;
};

// Structure to support turn-based active effect handling
struct FActiveTurnBasedEffectHandle {
public:
	FActiveGameplayEffectHandle ActiveEffectHandle;

	// -1 if the effect is permanent (like startup victory cells and kinging cells)
	int32 TurnsLeft;
};

USTRUCT(BlueprintType, Blueprintable)
struct FFateStoneContainer {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UFateStoneDataAsset> > DataAssets;

public:
	FORCEINLINE bool MoveAtIndexToAnother(int32 Index, FFateStoneContainer& Another) {
		if(DataAssets.IsValidIndex(Index)) 
		{
			// Will movement semantics be a problem?
			Another.DataAssets.Add(DataAssets[Index]);
			DataAssets.RemoveAt(Index);
		}
	}
};