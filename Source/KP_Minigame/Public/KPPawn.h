// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/KP_Structs.h"
#include "AbilitySystemInterface.h"
#include "KPPawn.generated.h"

class ABoardPiece;
class AKP_GameModeBase;
class ACell;
class UFateStoneDataAsset;
class UFateStonePlayerStoreComponent;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCanRollState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateSelectCell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateStepsCounter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateMovomentInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectFateStone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseOrCancelUseFateStone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateUseFateStoneState, bool, bCanUseFateStone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsingFateStoneDataRedy, bool, bRedy);

USTRUCT(Blueprintable)
struct FSelectedFateStoneData
{
	GENERATED_BODY()
	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	TWeakObjectPtr<const UFateStoneDataAsset> SelectedFateStone;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	int32 Index = -1;
};

UCLASS(Blueprintable, BlueprintType, abstract)
class KP_MINIGAME_API AKPPawn : public APawn, public IAbilitySystemInterface
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

	//Ability interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (AllowedClasses = "/Script/KP_Minigame.PlayerPawnGameplayAbilityBase"))
	TArray<TSoftClassPtr<UGameplayAbility> > DefaultAbilities;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	int32 PlayerId = -1;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateCanRollState OnUpdateCanRollState;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateSelectCell OnUpdateSelectCell;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateStepsCounter OnUpdateStepsCounter;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateMovomentInfo OnUpdateMovomentInfo;

	UPROPERTY(BlueprintAssignable)
	FOnSelectFateStone OnSelectFateStone;

	UPROPERTY(BlueprintAssignable)
	FOnUseOrCancelUseFateStone OnUseOrCancelUseFateStone;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateUseFateStoneState OnUpdateUseFateStoneState;

	UPROPERTY(BlueprintAssignable)
	FOnUsingFateStoneDataRedy OnUsingFateStoneDataRedy;

	void PreMakeStepData();
	void MakeStepData(const int32 StepPoints);
	void SetGameModePtr(AKP_GameModeBase* GM_Ptr);
	void InitBoardPieces(TArray<FKPPawnInfo>& PawnsInfo);
	void InitBoardPiece(FKPPawnInfo PawnInfo);
	void TerminateBoardPiece(FKPPawnInfo PawnInfo);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RollDices();

	void UpdateRollDicesData();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TurnEnd();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	bool CanRollDices() const; 

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TrySelectBoardPiece(ABoardPiece* BoardPiece);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RestSelectionCurrenBoardPiece();

	// call on GM  for Player or cool AI  for bot
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SelectCell(ACell* Cell);

	UFUNCTION(BlueprintPure, Category = Gameplay)
	bool CanMoveBoardPiece() const;

	UFUNCTION(BlueprintPure, Category = Gameplay)
	bool CanMoveToSelectedCell() const;

	UFUNCTION(BlueprintCallable, Category = Store)
	UFateStonePlayerStoreComponent* GetFateStoneStore() const ;

	UFUNCTION(BlueprintCallable, Category = Store)
	void InitFateStore(const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& InitData);

	UFUNCTION(BlueprintCallable, Category = Store)
	void SelectFateStone(int32 Index);

	// cancel use 
	UFUNCTION(BlueprintCallable, Category = Store)
	void CancelUsingFateStone();

	UFUNCTION(BlueprintCallable, Category = Store)
	void UseFateStone();

	void EndUseFateStone();

	void PrepareBoardToPlayer();

	UFUNCTION(BlueprintPure, Category = Store)
	bool CanGiveFateStone() const;

	UFUNCTION(BlueprintCallable, Category = Store)
	bool TryAddFateStone(UFateStoneDataAsset* FateStoneData);

protected:

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	TWeakObjectPtr<ABoardPiece> SelectedBoardPiece;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	int32 StepsCounter = 0;

	UPROPERTY(VisibleAnywhere, Category = Gameplay, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFateStonePlayerStoreComponent> FateStoneStore;

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<AKP_GameModeBase> GM;

	AKP_GameModeBase* GetKPGameMode() const;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly)
	bool bCanRollDices = false;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void MoveCurrentBoardPieceToSlectedCell();

	void ShowNavigationCellForCurentBoardPiece();
	void ClearNavigationCell();

	TArray<FBoardAtomicMovement> PossibleMovements;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	TWeakObjectPtr<ACell> SelectedCell = nullptr;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly, BlueprintReadOnly)
	FSelectedFateStoneData SelectedFateStoneData;

protected:
	EBoardPiece LastUsedBoardPieceTipe;
	void SetEnableFateStone(bool NewState);
private:
	bool bForFateStone = false;
};
