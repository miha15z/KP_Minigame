// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectFateStone.generated.h"

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UGameplayEffectFateStone : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fate Stone Effect")
	int32 Turns = 6;
};
