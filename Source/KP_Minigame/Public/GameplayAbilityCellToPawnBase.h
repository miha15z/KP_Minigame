// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/KP_Structs.h"
#include "GameplayAbilityCellToPawnBase.generated.h"


UCLASS(BlueprintType)
class UGameplayAbilityCellToPawnInfoHolder : public UObject {
	GENERATED_BODY()

	UPROPERTY()
	FGameplayAbilityCellToPawnInfo Info;
public:
	FORCEINLINE FGameplayAbilityCellToPawnInfo GetInfo() const {
		return Info;
	};

	FORCEINLINE void const SetInfo(FGameplayAbilityCellToPawnInfo NewInfo) {
		Info = NewInfo;
	};
};

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UGameplayAbilityCellToPawnBase : public UGameplayAbility
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayAbilityCellToPawnInfo GetInfo() const;
};
