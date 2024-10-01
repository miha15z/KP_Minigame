// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KP_GameModeBase.h"
#include "Core/Generators/GameBoardGeneratorBase.h"
#include "Core/Generators/Data/GenDataAsset.h"
#include "BoardNavigationSystem.h"
#include "Blueprint/UserWidget.h"
#include "KPPawn.h"
#include "BoardPiece.h"
#include "Cell.h"

AKP_GameModeBase* AKP_GameModeBase::GetKPGameMode(UObject* WorldContext)
{
    auto World = WorldContext->GetWorld();
    check(World);
    return World->GetAuthGameMode< AKP_GameModeBase>();
}

void AKP_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    //To do: Read DataAsset(navigation system, Generator,..) in Options

    check(!GenDataAsset.IsNull())
    GenDataAsset.LoadSynchronous();
    auto Generator = NewObject<UGameBoardGeneratorBase>(this);
    BoardData = Generator->GenerateGameBoard(GenDataAsset.Get(), this);

    BoardNavSystem = NewObject<UBoardNavigationSystem>(this);
    BoardNavSystem->SetupNeighbouringCellsByMask(BoardData.Cells, GenDataAsset->GetMovementPattern());


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
    for (auto Cell : BoardData.Cells)
    {
        if (Cell)
        {
            Cell->Reset();
            //to do
            //Cell->UpplyAbil
        }
    }

    for (auto& PlayerData : BoardData.PlayersData)
    {
        for (auto& PawnInfo : PlayerData.Pawns)
        {
           // PawnInfo.Pawn->
        }
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
        if (LastRollData.Value1 == LastRollData.Value2)
        {
            // to do: Get bonus
        }

        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Try reroll the dices, State is Lock"))
    }
    return false;
}

bool AKP_GameModeBase::IsWin_Implementation() const
{
    return false;
}

bool AKP_GameModeBase::EndTurn(AKPPawn* PlayerPawn)
{
    if (PlayerPawn != CurrentPawn)
    {
        return false;
    }

    if (IsWin())
    {
        check(CurrentPawn);
        OnWinKPGame.Broadcast(CurrentPawn->PlayerId);
        return false;
    }
    else
    {
        UpdateGameBoard();
        SelectNextPawn();
        OnFinishStep.Broadcast();
    }
    return true;
}

void AKP_GameModeBase::SelectNewBoardPiece(ABoardPiece* NewBoardPiece)
{
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
