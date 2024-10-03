// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */
namespace KP_GameplayTags
{
	KP_MINIGAME_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateWin);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateWin_KillKing);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateWin_ReachThePoint);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ShowWin);
}
