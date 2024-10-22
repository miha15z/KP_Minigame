// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeAbilities/GameModeAbility.h"
#include "GameplayAbilityGMDrawRandStone.generated.h"


class UFateStoneDataAsset;


/**
 * 
 */
UCLASS(abstract)
class KP_MINIGAME_API UGameplayAbilityGMDrawRandStone : public UGameModeAbility
{
	GENERATED_BODY()

	UGameplayAbilityGMDrawRandStone(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const override;
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Gameplay)
	int32 IndexStone = -1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Gameplay)
	const UFateStoneDataAsset* SelectedFateStoneData = nullptr;

};
