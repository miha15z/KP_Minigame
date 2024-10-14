// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPieceAbilities/GameplayAbility_DeadBoardPiece.h"
#include "KP_GameplayTags.h"

UGameplayAbility_DeadBoardPiece::UGameplayAbility_DeadBoardPiece(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_KillBoardPiece;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}
