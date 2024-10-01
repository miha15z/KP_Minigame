// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KP_Structs.h"
#include "KP_GameModeBase.generated.h"

class UBoardNavigationSystem;
class UGameBoardGeneratorBase;
class UGenDataAsset;
class UUserWidget;
class AKPPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRerollDices, FRollDicesData, RollResult, AKPPawn*, Player);
DECLARE_MULTICAST_DELEGATE(FOnFinishStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinKPGame, int32, PlayerId);
/**
 * 
 */
UCLASS()
class KP_MINIGAME_API AKP_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static AKP_GameModeBase* GetKPGameMode(UObject* WorldContext);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay();
	void UpdateGameBoard();
	//navigation
protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Board|Navigation")
	UBoardNavigationSystem* BoardNavSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Navigation")
	TSubclassOf<UBoardNavigationSystem> BoardNavSystemClass;
public:
	UFUNCTION(BlueprintCallable, Category ="Board|Navigation")
	UBoardNavigationSystem* GetBoardNavSystem() const { return BoardNavSystem; }

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

private:
	int32 RollDice() const;

public:
	UFUNCTION(BlueprintCallable, Category = RollDices)
	bool CanPlaerRollDices(AKPPawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = RollDices)
	void RerollDices(AKPPawn* PlayerPawn);

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
	bool EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SelectNewBoardPiece(ABoardPiece* NewBoardPiece);

	void SelectNextPawn();


};
