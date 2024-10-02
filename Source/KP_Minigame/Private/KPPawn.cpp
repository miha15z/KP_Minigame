// Fill out your copyright notice in the Description page of Project Settings.


#include "KPPawn.h"
#include "BoardPiece.h"
#include "Core/KP_GameModeBase.h"
#include "BoardNavigationSystem.h"
#include "Cell.h"

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
	GetKPGameMode()->EnableSelectabilityForBoardPieces(this, true);
}

void AKPPawn::SetGameModePtr(AKP_GameModeBase* GM_Ptr)
{
	GM = GM_Ptr;
}

void AKPPawn::InitBoardPieces(TArray<FKPPawnInfo>& PawnsInfo)
{
	for (auto& PawnInfo : PawnsInfo)
	{
		check(PawnInfo.Pawn);
		PawnInfo.Pawn->OnTrySelectBoardPiece.AddUObject(this, &AKPPawn::TrySelectBoardPiece);
	}
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
	RestSelectionCurrenBoardPiece();
	GetKPGameMode()->EnableSelectabilityForBoardPieces(this, false);
	GetKPGameMode()->EndTurn(this);
}

bool AKPPawn::CanRollDices() const
{
	return bCanRollDices;
}

void AKPPawn::TrySelectBoardPiece(ABoardPiece* BoardPiece)
{
	// TO DO :Check Can Select BoardPiece type
	RestSelectionCurrenBoardPiece();
	check(BoardPiece);
	BoardPiece->ConfirmSelection();
	LastUsedBoardPiece = BoardPiece;
	ClearNavigationCell();
	ShowNavigationCellForCurentBoardPiece();
}

void AKPPawn::RestSelectionCurrenBoardPiece()
{
	if (LastUsedBoardPiece.IsValid())
	{
		LastUsedBoardPiece->ResetSelection();
		LastUsedBoardPiece = nullptr;
	}
}

void AKPPawn::SelectCell(ACell* Cell)
{
	SelectedCell = Cell;
	OnUpdateSelectCell.Broadcast();
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

void AKPPawn::MoveCurrentBoardPieceToSlectedCell()
{
	check(LastUsedBoardPiece.IsValid() && SelectedCell.IsValid());
	// Updatecounter
	UBoardNavigationSystem* NavSys = GetKPGameMode()->GetBoardNavSystem();
	TArray<FBoardAtomicMovement> PathToCell;
	bool bMovementPossible;
	NavSys->GetMovementPathToCell(PossibleMovements, GetKPGameMode()->GetCellByID(LastUsedBoardPiece->GetCurrentCellId()), SelectedCell.Get(), bMovementPossible, PathToCell);
	StepsCounter -= FMath::CeilToInt((PathToCell.Last().MovementPointsConsumed + PathToCell.Last().MovementPointsLeft) - (PathToCell[0].MovementPointsLeft));

	GetKPGameMode()->LeaveCell(LastUsedBoardPiece->GetCurrentCellId(), LastUsedBoardPiece.Get());
	LastUsedBoardPiece->MoveToCell(SelectedCell->GetCellId(), SelectedCell->GetActorLocation());
	ABoardPiece** KilledPawn = nullptr;
	SelectedCell->PutPawnOnCell(LastUsedBoardPiece.Get(), KilledPawn);
	// if have kill - reset all steps
	if (KilledPawn != nullptr)
	{
		StepsCounter = 0;
	}
	ClearNavigationCell();
	RestSelectionCurrenBoardPiece();
	
}

void AKPPawn::ShowNavigationCellForCurentBoardPiece()
{
	UBoardNavigationSystem* NavSys = GetKPGameMode()->GetBoardNavSystem();

	// TODO:
	// MovementPoints Calculation
	NavSys->GetPossibleMovementsLocalData(LastUsedBoardPiece.Get(), StepsCounter, PossibleMovements);

	for (auto& MovementData : PossibleMovements) {
		MovementData.CellTo->SetState(ECellState::SelectToNav);
	}
}

void AKPPawn::ClearNavigationCell()
{
	for (auto& MovementData : PossibleMovements) {
		MovementData.CellTo->Reset();
	}
	// to do;
	SelectCell(nullptr);
}

