// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/KP_Structs.h"
#include "GameBoardGeneratorBase.generated.h"


class UGenDataAsset;
class ACellActorBase;
class UWorld;



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UGameBoardGeneratorBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Generation)
	FBoardData GenerateGameBoard(const UGenDataAsset* GenData, const UObject* WorldContext) const;
protected:
	virtual FBoardData GenerateGameBoard_Implementation(const UGenDataAsset* GenData, const UObject* WorldContext) const;
};
