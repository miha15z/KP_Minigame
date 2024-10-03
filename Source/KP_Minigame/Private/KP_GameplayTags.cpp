// Fill out your copyright notice in the Description page of Project Settings.


#include "KP_GameplayTags.h"

FGameplayTag KP_GameplayTags::FindTagByString(const FString& TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag& TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

namespace KP_GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateWin, "WinCondition", "Gloabls Win Conditions");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateWin_KillKing, "WinCondition.KillKing", "Win : Kill King");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateWin_ReachThePoint, "WinCondition.ReachThePoint", "Win : Reach the point ");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ShowWin, "WonPlayer", "Won player");
}
