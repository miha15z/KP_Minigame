// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellActorBase.generated.h"

UENUM(BlueprintType)
enum class ECellState : uint8
{
	None,
	SelectToNav,
	SelectToPlayer,
	ShowPath
};

UCLASS(abstract, Blueprintable, BlueprintType)
class KP_MINIGAME_API ACellActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACellActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void Reset();

	UFUNCTION(BlueprintCallable, Category = Cells)
	void SetState(ECellState NewState);

	UFUNCTION(BlueprintCallable, Category = Cells)
	FORCEINLINE ECellState GetCurrentState() const
	{
		return CurrentState;
	}

	UFUNCTION(BlueprintCallable, Category = Cells)
	FORCEINLINE int32 GetCellId() const
	{
		return CellId;
	}

public:
	UPROPERTY(BlueprintReadOnly, Category = CellInfo, BlueprintGetter = GetCellId)
	int32 CellId = 0;

protected:
	//UPROPERTY(EditDefaultsOnly, Category = CellInfo)
	//TSoftObjectPtr<UDataAsset> CellCongigData;

	UPROPERTY(EditDefaultsOnly, Category = CellInfo)
	ECellState CurrentState = ECellState::None;



};