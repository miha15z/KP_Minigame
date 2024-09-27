// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GenDataAsset.generated.h"



class ACellActorBase;
/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UGenDataAsset : public UDataAsset
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	int32 BoardSize = 8;

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	TSubclassOf<ACellActorBase> CellClass;

	UPROPERTY(EditDefaultsOnly, Category = GameBoard)
	TArray<int32> CellModifecation;
};
