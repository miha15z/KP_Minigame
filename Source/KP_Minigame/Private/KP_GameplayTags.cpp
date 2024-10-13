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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ActivateWin, "GameplayEvent.WinCondition", "Gloabls Win Conditions");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ActivateWin_KillKing, "GameplayEvent.WinCondition.KillKing", "Win : Kill King");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ActivateWin_ReachThePoint, "GameplayEvent.WinCondition.ReachThePoint", "Win : Reach the point ");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameState_ShowGameResult, "WonPlayer", "Won player");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_MoveBoardPiece, "GameplayEvent.BoardPiece.MoveToCell", "Move event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_KillBoardPiece, "GameplayEvent.BoardPiece.KellBoardPiece", "Kill board piece");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_UpgradePawn, "GameplayEvent.BoardPiece.UpgradePawn", "Upgrade pawn");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ApplyAbilitiesToBoardPiece, "GameplayEvent.Cell.ApplyToBoardPiece", "Apply to board piece");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_Move, "BoardPieceState.Move", "Move");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_None, "BoardPieceState.None", "None");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_Selectable, "BoardPieceState.Selectable", "Board piece is selectable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_Selected, "BoardPieceState.Selected", "Board piece selected");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_StartDead, "BoardPieceState.StartDead", "Start Dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_EndDead, "BoardPieceState.EndDead", "End Dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoardPieceState_Dead, "Dead", "Dead");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_BoardPiece_Move, "GameplayCue.MoveBoardPiece", "MoveBoardPieceCue")
}
