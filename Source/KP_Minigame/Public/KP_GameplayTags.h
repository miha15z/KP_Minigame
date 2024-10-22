// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */
namespace KP_GameplayTags
{
	KP_MINIGAME_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	KP_MINIGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_ActivateWin);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_ActivateWin_KillKing);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_ActivateWin_ReachThePoint);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RollDices);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_SelectFateStone);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_SelectRandomFateStone);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameState_ShowGameResult);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_MoveBoardPiece);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_KillBoardPiece);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_UpgradePawn);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_ApplyAbilitiesToBoardPiece);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_PlaceStoneInBag);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_EndTurn);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_Move);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_None);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_Selectable);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_Selected);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_StartDead);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_EndDead);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoardPieceState_Dead);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_BoardPiecce_PawnChooseToMove);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_BoardPiecce_KingChooseToMove);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_BoardPiece_Move);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_BoardPiece_PawnPromotion);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_BoardPiece_PawnKnockDown);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Basics_RollDices);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Basics_RollSameNumber);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Basics_TurnStart);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Basics_TurnEnd);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStone_OpenFateBag);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStone_DrawRandom);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStone_SwitchSet);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStone_PlaceInBag);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStone_Claim);
	
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FateStoneUse_Test);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Victory_KingReachCorner);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Victory_KingDefeat);

	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Ability_EndTurn);
	KP_MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Ability_StartTurn);
}
