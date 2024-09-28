// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNavigationSystem.generated.h"

class ACell;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API UBoardNavigationSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CalculateOrthogonalLength(ACell* Origin, ACell* Destination, int& Length);

};
