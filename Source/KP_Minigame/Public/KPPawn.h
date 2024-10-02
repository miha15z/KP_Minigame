// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/KP_Structs.h"
#include "KPPawn.generated.h"

class ABoardPiece;
class AKP_GameModeBase;
class ACell;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCanRollState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateSelectCell);


UCLASS(Blueprintable, BlueprintType, abstract)
class KP_MINIGAME_API AKPPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKPPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	int32 PlayerId = -1;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateCanRollState OnUpdateCanRollState;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateSelectCell OnUpdateSelectCell;

	bool CanSelectBoardPiece(ABoardPiece* BoardPiece) const;
	void PreMakeStepData();
	void MakeStepData(int32  StepPoints);
	void SetGameModePtr(AKP_GameModeBase* GM_Ptr);
	void InitBoardPieces(TArray<FKPPawnInfo>& PawnsInfo);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RollDices();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TurnEnd();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	bool CanRollDices() const; 

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TrySelectBoardPiece(ABoardPiece* BoardPiece);
	void RestSelectionCurrenBoardPiece();

	// call on GM  for Player or cool AI  for bot
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SelectCell(ACell* Cell);

protected:
	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	TWeakObjectPtr<ABoardPiece> LastUsedBoardPiece;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly)
	int32 StepsCounter = 0;

	UPROPERTY(Transient)
	TWeakObjectPtr<AKP_GameModeBase> GM;

	AKP_GameModeBase* GetKPGameMode();

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly)
	bool bCanRollDices = false;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void MoveCurrentBoardPieceToSlectedCell();

	void ShowNavigationCellForCurentBoardPiece();
	void ClearNavigationCell();

	TArray<FBoardAtomicMovement> PossibleMovements;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	TWeakObjectPtr<ACell> SelectedCell = nullptr;
};
