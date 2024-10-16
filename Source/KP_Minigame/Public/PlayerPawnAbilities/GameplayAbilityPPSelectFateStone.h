// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnAbilities/PlayerPawnGameplayAbilityBase.h"
#include "GameplayAbilityPPSelectFateStone.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KP_MINIGAME_API UGameplayAbilityPPSelectFateStone : public UPlayerPawnGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UGameplayAbilityPPSelectFateStone(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
