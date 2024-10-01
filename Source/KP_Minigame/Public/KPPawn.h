// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KPPawn.generated.h"

class ABoardPiece;
class AKP_GameModeBase;

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

	bool CanSelectBoardPiece(ABoardPiece* BoardPiece) const;
	void PreMakeStepData();
	void MakeStepData(int32  StepPoints);
	void EnableCanSelectedStateForBoardPieces();
	void SetGameModePtr(AKP_GameModeBase* GM_Ptr);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RollDices();

protected:
	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly)
	TWeakObjectPtr<ABoardPiece> LastUsedBoardPiece;

	UPROPERTY(Transient, Category = Gameplay, VisibleInstanceOnly)
	int32 StepsCounter = 0;

	UPROPERTY(Transient)
	TWeakObjectPtr<AKP_GameModeBase> GM;
};
