// Fill out your copyright notice in the Description page of Project Settings.


#include "KPPawn.h"
#include "BoardPiece.h"
#include "Core/KP_GameModeBase.h"
#include "BoardNavigationSystem.h"
#include "Cell.h"
#include "AbilitySystemComponent.h"
#include "KP_GameplayTags.h"
#include "FateStoneDataAsset.h"
#include "FateStonePlayerStoreComponent.h"
#include "KP_AbilitySystemComponent.h"
#include "GameplayAbilityFateStone.h"
#include "GameplayAbilitySpec.h"
#include "FateStoneColdStoreComponent.h"
#include "BoardPieceAttributeSet.h"

// Sets default values
AKPPawn::AKPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UKP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	FateStoneStore = CreateDefaultSubobject<UFateStonePlayerStoreComponent>(TEXT("FateStoneStore"));
	FateStoneColdStore = CreateDefaultSubobject<UFateStoneColdStoreComponent>(TEXT("FateStoneColdStore"));
}

// Called when the game starts or when spawned
void AKPPawn::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// add def  abilities
	for (auto& SoftAbilityClass : DefaultAbilities)
	{
		if (auto AbilityClass = SoftAbilityClass.LoadSynchronous())
		{
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this);;
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
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

void AKPPawn::PreMakeStepData()
{
	bCanRollDices = true;
	OnUpdateCanRollState.Broadcast();
}

void AKPPawn::MakeStepData(const int32 StepPoints)
{
	SelectedBoardPiece = nullptr;
	StepsCounter = StepPoints;
	GetKPGameMode()->EnableSelectabilityForBoardPieces(PlayerId, true);
	OnUpdateStepsCounter.Broadcast();
	LastUsedBoardPieceTipe = EBoardPiece::None;
	FateStoneStore->ResetNumberOfUse();
	OnUpdateUseFateStoneState.Broadcast(true);
	OnUsingFateStoneDataRedy.Broadcast(false);
}

void AKPPawn::SetGameModePtr(AKP_GameModeBase* GM_Ptr)
{
	GM = GM_Ptr;
}

void AKPPawn::InitBoardPieces(TArray<FKPPawnInfo>& PawnsInfo)
{
	for (auto& PawnInfo : PawnsInfo)
	{
		InitBoardPiece(PawnInfo);
	}
}

void AKPPawn::InitBoardPiece(FKPPawnInfo PawnInfo)
{
	check(PawnInfo.Pawn);
	PawnInfo.Pawn->OnTrySelectBoardPiece.AddUObject(this, &AKPPawn::TrySelectBoardPiece);
}

void AKPPawn::TerminateBoardPiece(FKPPawnInfo PawnInfo)
{
	if (PawnInfo.Pawn)
	{
		PawnInfo.Pawn->OnTrySelectBoardPiece.RemoveAll(this);
	}
}

void AKPPawn::RollDices()
{
	if (CanRollDices())
	{
		FGameplayEventData GameplayEventData;
		GameplayEventData.EventTag = KP_GameplayTags::GameplayEvent_MoveBoardPiece;
		GameplayEventData.Instigator = this;
		GM->GetAbilitySystemComponent()->HandleGameplayEvent(KP_GameplayTags::GameplayEvent_RollDices, &GameplayEventData);
		UpdateRollDicesData();
	}
}

void AKPPawn::UpdateRollDicesData() 
{
	const FRollDicesData RollData = GM->GetLastRollData();
	MakeStepData(RollData.Value1 > RollData.Value2 ? RollData.Value1 : RollData.Value2);
	bCanRollDices = false;
	OnUpdateCanRollState.Broadcast();
}

void AKPPawn::TurnEnd()
{
	RestSelectionCurrenBoardPiece();

	// reset steps
	StepsCounter = 0;
	OnUpdateStepsCounter.Broadcast();

	FGameplayEventData EventData;
	EventData.Instigator = this;
	GetKPGameMode()->GetAbilitySystemComponent()->HandleGameplayEvent(KP_GameplayTags::GameplayEvent_EndTurn, &EventData);

	OnUpdateUseFateStoneState.Broadcast(false);
}

bool AKPPawn::CanRollDices() const
{
	return bCanRollDices;
}

void AKPPawn::TrySelectBoardPiece(ABoardPiece* BoardPiece)
{
	if (GetKPGameMode()->GetCurrentPawn() != this)
	{
		return;
	}
	check(BoardPiece);
	if (!bForFateStone)
	{
		if (LastUsedBoardPieceTipe != EBoardPiece::None && BoardPiece->GetBoardPieceType() != LastUsedBoardPieceTipe)
		{
			// lock selection for next sub step
			return;
		}
	}

	RestSelectionCurrenBoardPiece();
	
	BoardPiece->ConfirmSelection();
	SelectedBoardPiece = BoardPiece;
	if (bForFateStone)
	{
		OnUsingFateStoneDataRedy.Broadcast(true);
	}
	else
	{
		ClearNavigationCell();
		ShowNavigationCellForCurentBoardPiece();
	}
}

void AKPPawn::RestSelectionCurrenBoardPiece()
{
	if (SelectedBoardPiece.IsValid())
	{
		SelectedBoardPiece->ResetSelection();
		SelectedBoardPiece = nullptr;
		OnUpdateMovomentInfo.Broadcast();
	}
}

void AKPPawn::SelectCell(ACell* Cell)
{
	if (GetKPGameMode()->GetCurrentPawn() != this)
	{
		return;
	}

	// set old cell's state if select other cell
	if (Cell && SelectedCell.IsValid())
	{
		SelectedCell->SetPastState();
	}

	SelectedCell = Cell;
	if (bForFateStone)
	{
		OnUsingFateStoneDataRedy.Broadcast(true);
	}
	//else
	{
		OnUpdateSelectCell.Broadcast();
		OnUpdateMovomentInfo.Broadcast();
	}
}

bool AKPPawn::CanMoveBoardPiece() const 
{
	// no steps
	if (StepsCounter <= 0)
	{
		return false;
	}

	bool  bCanMoveBoardPiece = false;
	UBoardNavigationSystem* NavSys = GetKPGameMode()->GetBoardNavSystem();
	TArray<FBoardAtomicMovement> TempMovement;

	auto CheckCanMoveLambda = [&](const TFunction<bool(const ABoardPiece*)>& Predicate)
		{
			for (const auto& PawnInfo : this->GetKPGameMode()->GetBoardPiecesForPlayer(PlayerId))
			{
				if (Predicate(PawnInfo.Pawn))
				{
					continue;
				}
				NavSys->GetPossibleMovementsLocalData(PawnInfo.Pawn, FMath::Min(StepsCounter, PawnInfo.Pawn->GetAvailableMovementPoints()), TempMovement);
				// TODO: Resolve through GetPossibleMovementsLocalData only
				if (TempMovement.Num() > 0 && PawnInfo.Pawn->GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsMaxAttribute()) > DBL_EPSILON)
				{
					UE_LOG(LogTemp, Display, TEXT("BoardPiece %s have %d PossibleMovements"), *PawnInfo.Pawn->GetHumanReadableName(), TempMovement.Num())
						bCanMoveBoardPiece = true;
					break;
				}
			}
		};

	if (LastUsedBoardPieceTipe == EBoardPiece::None)
	{
		//first step
		CheckCanMoveLambda([](const ABoardPiece* BoardPiece) 
			{
				return not BoardPiece->IsAlive(); 
			});
	}
	else
	{
		//second step, ...
		CheckCanMoveLambda([&](const ABoardPiece* BoardPiece) 
			{
				return not BoardPiece->IsAlive() || BoardPiece->GetBoardPieceType() != LastUsedBoardPieceTipe;
			});
	}
	return bCanMoveBoardPiece;
}

bool AKPPawn::CanMoveToSelectedCell() const
{
	return StepsCounter > 0 && SelectedCell.IsValid() && PossibleMovements.Num() > 0;
}

UFateStonePlayerStoreComponent* AKPPawn::GetFateStoneStore() const
{
	return FateStoneStore;
}

UFateStoneColdStoreComponent* AKPPawn::GetFateStonesColdStore() const
{
	return FateStoneColdStore;;
}

void AKPPawn::InitFateStore(const TArray<TSoftObjectPtr<UFateStoneDataAsset>>& InitData)
{
	FateStoneStore->Init(InitData);
}

void AKPPawn::SelectFateStone(const int32 Index)
{
	SelectedFateStoneData.SelectedFateStone = FateStoneStore->GetFateStone(Index);
	if (SelectedFateStoneData.SelectedFateStone.IsValid())
	{
		SelectedFateStoneData.Index = Index;
		OnSelectFateStone.Broadcast();
		
		const UGameplayAbilityFateStone* FateStoneAbilityCDO = GetDefault<UGameplayAbilityFateStone>(SelectedFateStoneData.SelectedFateStone->GetGameplayAbilityClass());
		if (FateStoneAbilityCDO)
		{
			// TODO: Add target class check

			// traverse cells
			int32 CurrentNumberOfUsePerTurn = FateStoneStore->GetCurrentNumberOfUseLeft();
			for (auto& Cell : GetKPGameMode() -> GetGameBoradData().Cells) 
			{
				if (IsValid(Cell) && !Cell->IsHidden()) 
				{
					bool bShouldSelectCell = FateStoneAbilityCDO->CanUseFateStone(Cell, this, CurrentNumberOfUsePerTurn);
					Cell->SetState(bShouldSelectCell ? ECellState::SelectToNav : ECellState::None);
				}
			}
			// traverse board pieces
			for (auto& Player : GetKPGameMode()->GetGameBoradData().PlayersData)
			{
				for (auto& BoardPieceInfo : Player.Pawns)
				{
					ABoardPiece* BoardPiece = BoardPieceInfo.Pawn;
					if (IsValid(BoardPiece) && BoardPiece->IsAlive())
					{
						bool bSouldSelectBoardPiece = FateStoneAbilityCDO->CanUseFateStone(BoardPiece, this, CurrentNumberOfUsePerTurn);
						BoardPiece->EnableSelectability(bSouldSelectBoardPiece);
					}
				}
			}
			// TODO?: Traverse Players
		}
		SetEnableFateStone(true);
	}
}

void AKPPawn::CancelUsingFateStone()
{
	SelectedFateStoneData.SelectedFateStone = nullptr;
	SelectedFateStoneData.Index = -1;
	OnUseOrCancelUseFateStone.Broadcast();
	SetEnableFateStone(false);
	SelectedCell = nullptr;
	SelectedBoardPiece = nullptr;
	OnUsingFateStoneDataRedy.Broadcast(false);
	PrepareBoardToPlayer();
}

void AKPPawn::UseFateStone()
{
	if (SelectedFateStoneData.SelectedFateStone.IsValid())
	{
		const UGameplayAbilityFateStone* FateStoneAbilityCDO = GetDefault<UGameplayAbilityFateStone>(SelectedFateStoneData.SelectedFateStone->GetGameplayAbilityClass());
		if (SelectedCell.IsValid() && FateStoneAbilityCDO->CanUseFateStone(SelectedCell.Get(), this, 1))
		{
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = SelectedCell.Get();
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(SelectedFateStoneData.SelectedFateStone->GetGameplayAbilityClass(), 1, -1, this);
			SelectedCell->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(Spec, &EventData);
			GM->ResetCells();
			SelectedCell = nullptr;
		}
		else if (SelectedBoardPiece.IsValid() && FateStoneAbilityCDO->CanUseFateStone(SelectedBoardPiece.Get(), this, 1))
		{
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = SelectedBoardPiece.Get();
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(SelectedFateStoneData.SelectedFateStone->GetGameplayAbilityClass(), 1, -1, this);
			SelectedBoardPiece->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(Spec, &EventData);
			SelectedBoardPiece->ResetSelection();
			SelectedBoardPiece = nullptr;
		}
		else
		{
			// !!!
			check(false);
		}

		// call in the baseAbility (End)
		//EndUseFateStone();
	}
}

void AKPPawn::EndUseFateStone()
{
	SetEnableFateStone(false);
	auto* FateStoneData = FateStoneStore->TryUseFateStone(SelectedFateStoneData.Index);
	check(FateStoneData);
	GM->AddFateStoneData(FateStoneData);
	OnUsingFateStoneDataRedy.Broadcast(false);
	PrepareBoardToPlayer();
	OnUseOrCancelUseFateStone.Broadcast();
	OnUpdateUseFateStoneState.Broadcast(FateStoneStore->CanUseFateStone());

	//Place a stone in the bag (Ability)
	FGameplayEventData GameplayEventData;
	GameplayEventData.EventTag = KP_GameplayTags::GameplayEvent_PlaceStoneInBag;
	GameplayEventData.Instigator = this;
	GameplayEventData.OptionalObject = FateStoneData;
	GetKPGameMode()->GetAbilitySystemComponent()->HandleGameplayEvent(KP_GameplayTags::GameplayEvent_PlaceStoneInBag, &GameplayEventData);
}

void AKPPawn::PrepareBoardToPlayer()
{
	GetKPGameMode()->ResetCells();
	GetKPGameMode()->EnableSelectabilityForBoardPiecesForAllPlayer(false);
	GetKPGameMode()->EnableSelectabilityForBoardPieces(PlayerId, true);
}

bool AKPPawn::CanGiveFateStone() const
{
	return not FateStoneStore->IsFull();;
}

bool AKPPawn::TryAddFateStone(UFateStoneDataAsset* FateStoneData)
{
	return FateStoneStore->TryAddFateStone(FateStoneData);
}

AKP_GameModeBase* AKPPawn::GetKPGameMode() const 
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
	check(SelectedBoardPiece.IsValid() && SelectedCell.IsValid());
	// Update counter
	UBoardNavigationSystem* NavSys = GetKPGameMode()->GetBoardNavSystem();
	TArray<FBoardAtomicMovement> PathToCell;
	bool bMovementPossible;
	NavSys->GetMovementPathToCell(PossibleMovements, GetKPGameMode()->GetCellByID(SelectedBoardPiece->GetCurrentCellId()), SelectedCell.Get(), bMovementPossible, PathToCell);
	const int32 PointsConsumed = FMath::CeilToInt((PathToCell.Last().MovementPointsConsumed + PathToCell.Last().MovementPointsLeft) - (PathToCell[0].MovementPointsLeft));
	StepsCounter -= PointsConsumed;
	// When the move happens, consume the available points for the pawn
	SelectedBoardPiece.Get()->ConsumeMovementPoints(PointsConsumed);

	GetKPGameMode()->LeaveCell(SelectedBoardPiece->GetCurrentCellId(), SelectedBoardPiece.Get());

	FGameplayEventData GameplayEventData;
	GameplayEventData.EventTag = KP_GameplayTags::GameplayEvent_MoveBoardPiece;
	GameplayEventData.Instigator = this;
	GameplayEventData.Target = SelectedCell.Get();
	SelectedBoardPiece->GetAbilitySystemComponent()->HandleGameplayEvent(KP_GameplayTags::GameplayEvent_MoveBoardPiece, &GameplayEventData);
	SelectedBoardPiece->MoveToCell(SelectedCell->GetCellId(), SelectedCell->GetActorLocation());

	ABoardPiece* KilledPawn = nullptr;
	SelectedCell->PutPawnOnCell(SelectedBoardPiece.Get(), &KilledPawn);
	SelectedCell->Reset();
	SelectedCell = nullptr;
	// if have kill - reset all steps

	if (KilledPawn != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Kill BoardPiece %s"),*(KilledPawn)->GetHumanReadableName());
		StepsCounter = 0;
		if ((KilledPawn)->GetBoardPieceType() == EBoardPiece::King)
		{
			UE_LOG(LogTemp, Display, TEXT("Kill King"));
			GetKPGameMode()->GetAbilitySystemComponent()->AddLooseGameplayTag(KP_GameplayTags::GameplayEvent_ActivateWin_KillKing);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Move"));
	}

	// save type  for lock other board piece types.
	LastUsedBoardPieceTipe = SelectedBoardPiece->GetBoardPieceType();

	ClearNavigationCell();
	RestSelectionCurrenBoardPiece();
	OnUpdateMovomentInfo.Broadcast();
	OnUpdateStepsCounter.Broadcast();
	GetKPGameMode()->CheckWinState();
}

void AKPPawn::ShowNavigationCellForCurentBoardPiece()
{
	UBoardNavigationSystem* NavSys = GetKPGameMode()->GetBoardNavSystem();
	// MovementPoints Calculation
	NavSys->GetPossibleMovementsLocalData(SelectedBoardPiece.Get(), FMath::Min(StepsCounter, SelectedBoardPiece.Get()->GetAvailableMovementPoints()), PossibleMovements);

	for (auto& MovementData : PossibleMovements)
	{
		if (SelectedBoardPiece->GetBoardPieceType() == EBoardPiece::King)
		{
			MovementData.CellTo->SetState((MovementData.bSafeForKing) ? ECellState::SelectToNav : ECellState::None);
		}
		else
		{
			MovementData.CellTo->SetState(ECellState::SelectToNav);
		}
	}
}

void AKPPawn::ClearNavigationCell()
{
	for (auto& MovementData : PossibleMovements) 
	{
		MovementData.CellTo->Reset();
	}

	PossibleMovements.Empty();
	SelectCell(nullptr);
}

void AKPPawn::SetEnableFateStone(const bool NewState)
{
	bForFateStone = NewState;
	FString PrintStr (bForFateStone ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Display, TEXT("bForFateStone = %s"), *PrintStr);
}

