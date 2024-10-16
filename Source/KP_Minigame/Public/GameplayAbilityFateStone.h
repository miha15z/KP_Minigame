// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityFateStone.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, abstract)
class KP_MINIGAME_API UGameplayAbilityFateStone : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="Fate Stone")
	TArray<TSubclassOf<AActor>> TargetClasses;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanUseFateStone(const AActor* Target, const AActor* Owner, int32 FateStoneActions = 0) const;
	virtual bool CanUseFateStone_Implementation(const AActor * Target, const AActor * Owner, int32 FateStoneActions) const;

	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<AActor>> GetTargetClasses() const ;

};



