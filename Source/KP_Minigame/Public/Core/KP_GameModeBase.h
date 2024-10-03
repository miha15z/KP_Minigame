// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KP_Structs.h"
#include "AbilitySystemInterface.h"
#include "KP_GameModeBase.generated.h"

class UBoardNavigationSystem;
class UGameBoardGeneratorBase;
class UGenDataAsset;
class UUserWidget;
class AKPPawn;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRerollDices, FRollDicesData, RollResult, AKPPawn*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinKPGame, int32, PlayerId);

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API AKP_GameModeBase : public AGameModeBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static AKP_GameModeBase* GetKPGameMode(UObject* WorldContext);

	AKP_GameModeBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay();
	void UpdateGameBoard();
	void ResetCells();
	// Resets Board Pieces at the end of the turn
	void ResetBoardPieces();
	//tags

	//navigation
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "Board|Navigation")
	UBoardNavigationSystem* BoardNavSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Navigation")
	TSubclassOf<UBoardNavigationSystem> BoardNavSystemClass;

	UFUNCTION()
	void SelectCellForCurrentPlayer(ACell* Cell);
public:
	UFUNCTION(BlueprintCallable, Category ="Board|Navigation")
	UBoardNavigationSystem* GetBoardNavSystem() const { return BoardNavSystem; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	//generator
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSubclassOf<UGameBoardGeneratorBase> BoardGeneratorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSoftObjectPtr<UGenDataAsset> GenDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Generator")
	TSubclassOf<AKPPawn> BotPawnClass;

	//GameBoard
	UPROPERTY(Transient)
	FBoardData BoardData;

public:
	UFUNCTION(BlueprintCallable, Category = Gameboard)
	const FBoardData& GetGameBoradData() const {return BoardData;}

	UFUNCTION(BlueprintCallable, Category = Gameboard)
	FRollDicesData GetLastRollData()const;
	//UI
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UUserWidget> UIClass;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* PlayerUI;

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

private:
	int32 RollDice() const;

public:
	UFUNCTION(BlueprintCallable, Category = RollDices)
	bool CanPlaerRollDices(AKPPawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = RollDices)
	bool RerollDices(AKPPawn* PlayerPawn);

	ACell* GetCellByID(int32 ID) const;

	//Gameplay
protected:
	//Maybe use TQueue
	//UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
	TQueue<AKPPawn*> QueuePawns;
	AKPPawn* CurrentPawn = nullptr;
	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
	bool IsWin() const;
	virtual bool IsWin_Implementation()const;


public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool EndTurn(AKPPawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SelectNewBoardPiece(ABoardPiece* NewBoardPiece);

	void SelectNextPawn();

	UFUNCTION(BlueprintCallable, Category = RollDices)
	AKPPawn* GetCurrentPawn()const;

	void EnableSelectabilityForBoardPieces(AKPPawn* OwnerPlayer, bool NewState, EBoardPiece BoardPieceType = EBoardPiece::None);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void  LeaveCell(int32 CellId, ABoardPiece* BoardPiece);


};
