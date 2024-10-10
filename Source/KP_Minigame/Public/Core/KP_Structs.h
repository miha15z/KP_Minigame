// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// Is it an overkill?
#include "AbilitySystemComponent.h"
#include "KP_Structs.generated.h"

class ACell;
class ABoardPiece;
class UGameplayAbilityCellToPawnBase;
class UFateStoneData;

USTRUCT(BlueprintType)
struct FBoardCoord
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 y;

	FBoardCoord operator*(FBoardCoord const& other) const {
		return FBoardCoord{ (this->x * other.x), (this->y * other.y) };
	}

	FBoardCoord operator-(FBoardCoord const& other) const {
		return FBoardCoord{ (this->x - other.x), (this->y - other.y) };
	}

	bool operator==(FBoardCoord const& other) const {
		return (this->x == other.x) and (this->y == other.y);
	}
};

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

// Structure for storing UGameplayAbilityCellToPawnBase Information both for setup and ability activation
USTRUCT(BlueprintType)
struct FGameplayAbilityCellToPawnInfo {
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbilityCellToPawnBase> AbilityClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int OwningPlayerID;
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

	UPROPERTY(EditInstanceOnly)
	TArray<TSoftObjectPtr<UFateStoneData>> StartupFateStones;
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
	int32 Value1;
	UPROPERTY(BlueprintReadOnly)
	int32 Value2;
};

USTRUCT(BlueprintType, Blueprintable)
struct FBoardAtomicMovement {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	ACell* CellTo;
	UPROPERTY(BlueprintReadOnly)
	ACell* CellFrom;
	UPROPERTY()
	float MovementPointsLeft;
	UPROPERTY()
	float MovementPointsConsumed;
};

// Structure to support turn-based active effect handling
struct FActiveTurnBasedEffectHandle {
public:
	FActiveGameplayEffectHandle ActiveEffectHandle;

	// -1 if the effect is permanent (like startup victory cells and kinging cells)
	int32 TurnsLeft;
};