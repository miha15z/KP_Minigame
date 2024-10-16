// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SelectFateStoneHolder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API USelectFateStoneHolder : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 FateStoneIndex = -1;
	
};
