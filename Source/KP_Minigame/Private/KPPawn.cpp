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
	if (GM.IsValid())
	{
		GM->RerollDices(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NotValid GM in %s"), *GetHumanReadableName());
	}
}

