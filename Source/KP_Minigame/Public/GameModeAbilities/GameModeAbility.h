// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameModeAbility.generated.h"


class AKPPawn;
/**
 * 
 */
UCLASS(abstract)
class KP_MINIGAME_API UGameModeAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = GameplayCue)
	bool bAutoActivateGameplayCue = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GA_Data)
	TWeakObjectPtr<AKPPawn> Pawn = nullptr;
	
};
