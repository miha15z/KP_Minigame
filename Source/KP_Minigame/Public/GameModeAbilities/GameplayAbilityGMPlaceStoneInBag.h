// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeAbilities/GameModeAbility.h"
#include "GameplayAbilityGMPlaceStoneInBag.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KP_MINIGAME_API UGameplayAbilityGMPlaceStoneInBag : public UGameModeAbility
{
	GENERATED_BODY()
public:
	UGameplayAbilityGMPlaceStoneInBag(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/* TriggerEventData->Instigator - AKPPawn;
	*  TriggerEventData->OptionalObject - UFateStoneDataAsset*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
