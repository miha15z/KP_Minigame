// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KP_GameModeBase.h"
#include "Core/Generators/GameBoardGeneratorBase.h"
#include "Core/Generators/Data/GenDataAsset.h"
#include "BoardNavigationSystem.h"
#include "Blueprint/UserWidget.h"
#include "KPPawn.h"
#include "BoardPiece.h"
#include "Cell.h"
#include "KP_GameplayTags.h"
#include "KP_AbilitySystemComponent.h"
#include "AbilitySystemComponent.h"

AKP_GameModeBase* AKP_GameModeBase::GetKPGameMode(UObject* WorldContext)
{
    auto World = WorldContext->GetWorld();
    check(World);
    return World->GetAuthGameMode< AKP_GameModeBase>();
}

AKP_GameModeBase::AKP_GameModeBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
    AbilitySystemComponent = CreateDefaultSubobject<UKP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    WinTags.AddTag(KP_GameplayTags::Ability_ActivateWin);
}

void AKP_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    //To do: Read DataAsset(navigation system, Generator,..) in Options

    check(!GenDataAsset.IsNull())
    GenDataAsset.LoadSynchronous();
    auto Generator = NewObject<UGameBoardGeneratorBase>(this);
    BoardData = Generator->GenerateGameBoard(GenDataAsset.Get(), this);

    BoardNavSystem = NewObject<UBoardNavigationSystem>(this);
    BoardNavSystem->Init(&BoardData);
    BoardNavSystem->SetupNeighbouringCellsByMask(BoardData.Cells, GenDataAsset->GetMovementPattern());

    for (auto& Cell : BoardData.Cells)
    {
        Cell->OnSelectCell.AddUObject(this, &AKP_GameModeBase::SelectCellForCurrentPlayer);
    }

    Super::InitGame(MapName, Options, ErrorMessage);
}

void AKP_GameModeBase::StartPlay()
{
    Super::StartPlay();
    check(BotPawnClass.Get())
    // Select first player
    auto World = GetWorld();
    check(World);
    auto PlayerPawn = World->GetFirstPlayerController()->GetPawn<AKPPawn>();
    check(PlayerPawn);
    PlayerPawn->PlayerId = 0;
    PlayerPawn->SetGameModePtr(this);
    PlayerPawn->InitBoardPieces(BoardData.PlayersData[0].Pawns);
    // Setup Startup fate stones
    PlayerPawn->OwnedFateStones = GenDataAsset.Get()->GetPlayerData(0).StartupFateStones;
    //Make GameQueue
    QueuePawns.Enqueue(PlayerPawn);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    const FTransform SpawnTransform;
    for (int32 i = 1; i < BoardData.PlayersData.Num(); ++i)
    {
        auto PawnAI = World->SpawnActor<AKPPawn>(BotPawnClass.Get(), SpawnTransform, SpawnParams);
        PawnAI->PlayerId = i;
        PawnAI->SetGameModePtr(this);
        PawnAI->InitBoardPieces(BoardData.PlayersData[i].Pawns);
        QueuePawns.Enqueue(PawnAI);
    }
    UpdateGameBoard();
    SelectNextPawn();

	//ui
	PlayerUI = CreateWidget(this->GetWorld(), UIClass);
	PlayerUI->AddToViewport();

    // firstInitUI and other
    OnFinishStep.Broadcast();
}

void AKP_GameModeBase::UpdateGameBoard()
{
    ResetCells();

    //  to do: update pawns, apply abilities, update steps counters 
    //for (auto& PlayerData : BoardData.PlayersData)
    //{
    //    for (auto& PawnInfo : PlayerData.Pawns)
    //    {
    //       // PawnInfo.Pawn->
    //    }
    //}
}

void AKP_GameModeBase::ResetCells()
{
    for (auto Cell : BoardData.Cells)
    {
        if (Cell)
        {
            Cell->Reset();
        }
    }
}

void AKP_GameModeBase::ResetBoardPieces()
{
    for (auto Player : BoardData.PlayersData)
    {
        for (auto BoardPiece : Player.Pawns) 
        {
            BoardPiece.Pawn->ResetAvailableMovementPoints();
        }
    }
}

void AKP_GameModeBase::SelectCellForCurrentPlayer(ACell* Cell)
{
    if (IsValid(CurrentPawn))
    {
        CurrentPawn->SelectCell(Cell);
    }
}

FRollDicesData AKP_GameModeBase::GetLastRollData() const
{
    return LastRollData;
}

int32 AKP_GameModeBase::RollDice() const
{
    return  FMath::RandRange(RollDiscesMinValue, RollDiscesMaxValue);
}

bool AKP_GameModeBase::CanPlaerRollDices(AKPPawn* PlayerPawn)
{
    return PlayerPawn == CurrentPawn ; //  to do
}

bool AKP_GameModeBase::RerollDices(AKPPawn* PlayerPawn)
{
    if (CanPlaerRollDices(PlayerPawn))
    {
        LastRollData.Value1 = RollDice();
        LastRollData.Value2 = RollDice();
        OnRerollDices.Broadcast(LastRollData, PlayerPawn);

        //if (LastRollData.Value1 == LastRollData.Value2)
        //{
        //    // to do: Get bonus
        //}

        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Try reroll the dices, State is Lock"))
    }
    return false;
}

ACell* AKP_GameModeBase::GetCellByID(int32 Id) const
{
    return BoardData.GetGellByIdChecked(Id);
}

bool AKP_GameModeBase::IsWin_Implementation() const
{
    check(AbilitySystemComponent);
    return  AbilitySystemComponent->HasAnyMatchingGameplayTags(WinTags);
}

bool AKP_GameModeBase::EndTurn(AKPPawn* PlayerPawn)
{
    if (PlayerPawn != CurrentPawn)
    {
        return false;
    }

    // Decrease fate stone turn counter
    Cast<UKP_AbilitySystemComponent>(AbilitySystemComponent)->DecreaseActiveTurnBasedEffectsCounters();

    // Resets Board Pieces at the end of the turn
    ResetBoardPieces();

    if(not CheckWinState())
    {
        UpdateGameBoard();
        SelectNextPawn();
        OnFinishStep.Broadcast();
    }
    return true;
}

bool AKP_GameModeBase::CheckWinState() const
{
    if (IsWin())
    {
        check(CurrentPawn);
        OnWinKPGame.Broadcast(CurrentPawn->PlayerId);
        return true;
    }
    else
    {
        return false;
    }
}

void AKP_GameModeBase::SelectNextPawn()
{
    if(IsValid(CurrentPawn))
    {
        QueuePawns.Enqueue(CurrentPawn);
    }
    QueuePawns.Dequeue(CurrentPawn);
    check(CurrentPawn);
    CurrentPawn->PreMakeStepData();
}

AKPPawn* AKP_GameModeBase::GetCurrentPawn() const
{
    return CurrentPawn;
}

void AKP_GameModeBase::EnableSelectabilityForBoardPieces(const AKPPawn * OwnerPlayer, const bool NewState, const EBoardPiece BoardPieceType)
{
    check(OwnerPlayer);
    check(BoardData.PlayersData.Num() > OwnerPlayer->PlayerId);
    // apply for all
    if (BoardPieceType == EBoardPiece::None)
    {
        for (auto& PawnsInfo : BoardData.PlayersData[OwnerPlayer->PlayerId].Pawns)
        {
            auto* BoardPiece = PawnsInfo.Pawn;
            if (IsValid(BoardPiece) && BoardPiece->IsAlive())
            {
                BoardPiece->EnableSelectability(NewState);
            }
        }
    }
    // apply for type
    else
    {
        for (auto& PawnsInfo : BoardData.PlayersData[OwnerPlayer->PlayerId].Pawns)
        {
            auto* BoardPiece = PawnsInfo.Pawn;
            if (IsValid(BoardPiece) && BoardPiece->IsAlive() && BoardPiece->GetBoardPieceType() == BoardPieceType)
            {
                BoardPiece->EnableSelectability(NewState);
            }
        }
    }
}

void AKP_GameModeBase::LeaveCell(int32 CellId, ABoardPiece* BoardPiece)
{
    BoardData.GetGellByIdChecked(CellId)->LeavePawn(BoardPiece);
}
