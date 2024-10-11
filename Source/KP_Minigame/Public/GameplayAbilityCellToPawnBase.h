// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/KP_Structs.h"
#include "GameplayAbilityCellToPawnBase.generated.h"



// UObject to hold FGameplayAbilityCellToPawnInfo (payload) when transferring inside the activating UGameplayAbilityCellToPawnBase
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
UCLASS(abstract)
class KP_MINIGAME_API UGameplayAbilityCellToPawnBase : public UGameplayAbility
{
	GENERATED_BODY()
	
	// Get Payload Info from inside the Ability
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayAbilityCellToPawnInfo GetInfo() const;
};
