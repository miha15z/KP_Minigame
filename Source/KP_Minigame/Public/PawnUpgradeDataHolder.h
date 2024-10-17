// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PawnUpgradeDataHolder.generated.h"

class ABoardPiece;

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UPawnUpgradeDataHolder : public UObject
{
	GENERATED_BODY()
public:

public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABoardPiece> UpgradeClass;
};
