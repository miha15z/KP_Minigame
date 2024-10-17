// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeAbilities/GameModeAbility.h"
#include "GameplayAbilityGMUpgradePawn.generated.h"

class ABoardPiece;
/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UGameplayAbilityGMUpgradePawn : public UGameModeAbility
{
	GENERATED_BODY()

public:
	// TODO: Remove, Make caller specify it
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABoardPiece> UpgradeClass;

public:
	UGameplayAbilityGMUpgradePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
