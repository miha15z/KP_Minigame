// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KP_Structs.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "KP_GameModeBase.generated.h"

class UBoardNavigationSystem;
class UGameBoardGeneratorBase;
class UGenDataAsset;
class UUserWidget;
class AKPPawn;
class UAbilitySystemComponent;
class UGameplayAbility;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRerollDices, FRollDicesData, RollResult, AKPPawn*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinKPGame, int32, PlayerId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGiveBonus, int32, PlayerId);

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API AKP_GameModeBase : public AGameModeBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AKP_GameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UFUNCTION(BlueprintCallable)
	static AKP_GameModeBase* GetKPGameMode(const UObject* WorldContext);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay();
	void UpdateGameBoard();
	void ResetCells();
	// Resets Board Pieces at the end of the turn
	void ResetBoardPieces();
protected:
	virtual void BeginPlay() override;

	//navigation
protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Board|Navigation")
	TObjectPtr<UBoardNavigationSystem> BoardNavSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Navigation")
	TSoftClassPtr<UBoardNavigationSystem> BoardNavSystemClass;

	UFUNCTION()
	void SelectCellForCurrentPlayer(ACell* Cell);

public:
	UFUNCTION(BlueprintCallable, Category ="Board|Navigation")
	UBoardNavigationSystem* GetBoardNavSystem() const { return BoardNavSystem; }

	//AbilitySystem
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (AllowedClasses = "/Script/KP_Minigame.GameModeAbility"))
	TArray<TSoftClassPtr<UGameplayAbility> > DefaultAbilities;
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	//Stones of Fate
protected:
	//This store is used for gameplay
	UPROPERTY(VisibleAnywhere, Category = Gameplay, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFateStonePlayerStoreComponent> FateStoneStore;

	// You can enable or disable Drawing random stone of fate for tests or chege gameplay 
	UPROPERTY(EditDefaultsOnly, Category = Gameplay, BlueprintReadWrite)
	bool bDrawingRandomFateStone = false;
public:

	UFUNCTION(BlueprintCallable, Category = Store)
	void InitFateStore(const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& InitData);

	UFUNCTION(BlueprintCallable, Category = Store)
	UFateStonePlayerStoreComponent* GetFateStoneStore() const;

	UFUNCTION(BlueprintCallable, Category = Store)
	bool TryCurrentPawnGiveFateStone(const int32 FateStoneId);

	UFUNCTION(BlueprintCallable, Category = Store)
	bool AddFateStoneData(UFateStoneDataAsset* Data);

	UFUNCTION(BlueprintPure, Category = Store)
	bool IsDrawingRandomFateStone() const;

	UFUNCTION(BlueprintPure, Category = Store)
	bool CanGiveFateStone(const AKPPawn* Pawn);

	// return -1 if don't have stones or return stone's index
	UFUNCTION(BlueprintCallable, Category = Store)
	int32 GetRandomFateStoneIndex() const;
	
	UFUNCTION(BlueprintCallable, Category = Store)
	const UFateStoneDataAsset* GetFateStoneData(const int32 Index);

	//generator
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSoftClassPtr<UGameBoardGeneratorBase> BoardGeneratorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSoftObjectPtr<UGenDataAsset> GenDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSoftClassPtr<AKPPawn> BotPawnClass;

	//GameBoard
	UPROPERTY(Transient)
	FBoardData BoardData;

public:
	UFUNCTION(BlueprintCallable, Category = Gameboard)
	const FBoardData& GetGameBoradData() const {return BoardData;}

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	const FRollDicesData& GetLastRollData()const;

	UFUNCTION(BlueprintPure, Category = Gameplay)
	bool IsBonusRollData() const;

	void TryGiveBonus();
	void ShowGiveBonusUI() const;
	//may be used coldStore
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	const TArray<TSoftObjectPtr<UFateStoneDataAsset> >& GetStonesInGameForPlayer(const int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	TSoftObjectPtr<UFateStoneDataAsset> RemoveFateStoneInGameForPlayer(const int32 IndexStone, const int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void AddFateStoneInGameForPlayer(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData, const int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TakePlayersFateStones();

	//UI
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSoftClassPtr<UUserWidget> UIClass;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSoftClassPtr<UUserWidget> UISelectStonesToGameClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PlayerUI;

protected:
	//Roll the dices
	UPROPERTY(VisibleInstanceOnly, Category = RollDices)
	FRollDicesData LastRollData;
	
	UPROPERTY(EditDefaultsOnly, Category = RollDices);
	int32 RollDiscesMinValue = 1;

	UPROPERTY(EditDefaultsOnly, Category = RollDices);
	int32 RollDiscesMaxValue = 3;

public:
	UPROPERTY(BlueprintAssignable, Category = RollDices)
	FOnRerollDices OnRerollDices;
	UPROPERTY(BlueprintAssignable, Category = RollDices)
	FOnWinKPGame OnWinKPGame;
	UPROPERTY(BlueprintAssignable, Category = RollDices)
	FOnFinishStep OnFinishStep;
	UPROPERTY(BlueprintAssignable, Category = Gameplay)
	FOnGiveBonus OnGiveBonus;
	UPROPERTY(BlueprintAssignable, Category = Gameplay)
	FOnFinishStep OnBonusTaken;

private:
	int32 RollDice() const;

public:
	UFUNCTION(BlueprintCallable, Category = RollDices)
	bool CanPlaerRollDices(const AKPPawn* PlayerPawn) const;

	void OnRollDices() const;

	void RollDices();

	ACell* GetCellByID(const int32 Id) const;

	//Gameplay
protected:

	TQueue<TWeakObjectPtr<AKPPawn>> QueuePawns;
	TArray<TWeakObjectPtr<AKPPawn>> Pawns;
	TWeakObjectPtr<AKPPawn> CurrentPawn = nullptr;

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
	bool IsWin() const;
	virtual bool IsWin_Implementation()const;

	UPROPERTY(VisibleAnywhere, Category = Gameplay, Transient)
	int32 StepsCounter = 0;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	FGameplayTagContainer WinTags;

public:
	UFUNCTION(BlueprintPure, Category = Gameplay)
	FORCEINLINE int32 GetCurrentStep() const 
	{
		return StepsCounter;
	}

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool EndTurn(AKPPawn* PlayerPawn);

	bool CheckWinState() const ;

	void SelectNextPawn();

	UFUNCTION(BlueprintCallable, Category = RollDices)
	AKPPawn* GetCurrentPawn() const;

	UFUNCTION(BlueprintCallable, Category = Player)
	AKPPawn* GetPlayerPawnById(const int32 Id) const;

	void EnableSelectabilityForBoardPieces(const int32 OwnerPlayerId, const bool NewState, const EBoardPiece BoardPieceType = EBoardPiece::None);
	void EnableSelectabilityForBoardPiecesForAllPlayer(const bool NewState, const EBoardPiece BoardPieceType = EBoardPiece::None);
	void EnableSelectabilityForBoardPiecesForOtherPlayers(const int32 PlayerId, const bool NewState, const EBoardPiece BoardPieceType = EBoardPiece::None);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void  LeaveCell(const int32 CellId, const ABoardPiece * BoardPiece);

	const TArray<FKPPawnInfo>& GetBoardPiecesForPlayer(const int32 PlayerId) const;


};
