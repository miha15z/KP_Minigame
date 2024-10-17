// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAbilities/GameplayAbilityGMUpgradePawn.h"
#include "KP_GameplayTags.h"
#include "BoardPiece.h"
#include "Core/KP_GameModeBase.h"
#include "Cell.h"
#include "KPPawn.h"

UGameplayAbilityGMUpgradePawn::UGameplayAbilityGMUpgradePawn(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = KP_GameplayTags::GameplayEvent_UpgradePawn;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbilityGMUpgradePawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Replace the existing pawn with the new one
	ABoardPiece* InitialPawn = Cast<ABoardPiece>(const_cast<AActor*>(TriggerEventData->Target.Get()));
	check(InitialPawn && GetWorld())
	
	if (InitialPawn->StaticClass() == UpgradeClass)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
	FVector InitialActorLocation = InitialPawn->GetActorLocation();
	FRotator InitialActorRotation = InitialPawn->GetActorRotation();

	ABoardPiece* NewPawn = GetWorld()->SpawnActor<ABoardPiece>(UpgradeClass, InitialActorLocation, InitialActorRotation);
	NewPawn->CopyState(InitialPawn);

	// Replace all outer references to the old pawn with references to the new one
	auto* GM = Cast<AKP_GameModeBase>(GetOwningActorFromActorInfo());
	check(GM);

	ACell* LocationCell = GM->GetCellByID(NewPawn->GetCurrentCellId());
	LocationCell->ReplaceByPawn(NewPawn);

	TArray<FKPPawnInfo>& PawnsArray = const_cast<FBoardData&>(GM->GetGameBoradData()).PlayersData[NewPawn->GetOwnPlayerId()].Pawns;
	for (auto& PawnsInfo : PawnsArray)
	{
		if (PawnsInfo.Pawn == InitialPawn)
		{
			GM->GetPlayerPawnById(NewPawn->GetOwnPlayerId())->TerminateBoardPiece(PawnsInfo);
			PawnsInfo.Pawn = NewPawn;
			GM->GetPlayerPawnById(NewPawn->GetOwnPlayerId())->InitBoardPiece(PawnsInfo);
			break;
		}
	}

	// Deactivate Initial Pawn
	InitialPawn->ForceKill();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


