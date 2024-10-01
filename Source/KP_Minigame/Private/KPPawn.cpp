// Fill out your copyright notice in the Description page of Project Settings.


#include "KPPawn.h"
#include "BoardPiece.h"
#include "Core/KP_GameModeBase.h"

// Sets default values
AKPPawn::AKPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AKPPawn::CanSelectBoardPiece(ABoardPiece* BoardPiece) const
{
	return false;
}

void AKPPawn::PreMakeStepData()
{
	bCanRollDices = true;
	OnUpdateCanRollState.Broadcast();
}

void AKPPawn::MakeStepData(int32 StepPoints)
{
	LastUsedBoardPiece = nullptr;
	StepsCounter = StepPoints;

}

void AKPPawn::EnableCanSelectedStateForBoardPieces()
{
}

void AKPPawn::SetGameModePtr(AKP_GameModeBase* GM_Ptr)
{
	GM = GM_Ptr;
}

void AKPPawn::RollDices()
{
	if (CanRollDices() && GetKPGameMode()->RerollDices(this))
	{
		auto RollData = GM->GetLastRollData();
		MakeStepData(RollData.Value1 > RollData.Value2 ? RollData.Value1 : RollData.Value2);
		bCanRollDices = false;
		OnUpdateCanRollState.Broadcast();
	}
}

void AKPPawn::TurnEnd()
{
	GetKPGameMode()->EndTurn(this);
}

bool AKPPawn::CanRollDices() const
{
	return bCanRollDices;
}

AKP_GameModeBase* AKPPawn::GetKPGameMode()
{
	if (GM.IsValid())
	{ 
		return GM.Get();
	}
	else
	{
		GM = AKP_GameModeBase::GetKPGameMode(this);
		check(GM.IsValid());
	}
	return GM.Get();
}

