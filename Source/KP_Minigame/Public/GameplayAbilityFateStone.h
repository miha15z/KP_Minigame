// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityFateStone.generated.h"

class AKPPawn;

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

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	TWeakObjectPtr<AKPPawn> Instigator;

};



