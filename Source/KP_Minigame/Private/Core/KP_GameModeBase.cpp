// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KP_GameModeBase.h"
#include "Core/Generators/GameBoardGeneratorBase.h"
#include "Core/Generators/Data/GenDataAsset.h"
#include "BoardNavigationSystem.h"
#include "Blueprint/UserWidget.h"
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
    auto Generator = NewObject<UGameBoardGeneratorBase>();
    BoardData = Generator->GenerateGameBoard(GenDataAsset.Get(), this);

    BoardNavSystem = NewObject<UBoardNavigationSystem>();
    BoardNavSystem->SetupNeighbouringCellsByMask(BoardData.Cells, GenDataAsset->GetMovementPattern());

    //ui
    PlayerUI = CreateWidget(this->GetWorld(), UIClass);
    PlayerUI->AddToViewport();
    Super::InitGame(MapName, Options, ErrorMessage);
}

void AKP_GameModeBase::StartPlay()
{
    Super::StartPlay();

    for (auto Cell : BoardData.Cells)
    {
        if (Cell)
        {
            Cell->Reset();
        }
    }

    // Select first player
    auto PlayerPawn = GetWorld() -> GetFirstPlayerController()->GetPawn();
    check(PlayerPawn);
}

int32 AKP_GameModeBase::RollDice() const
{
    return  FMath::RandRange(RollDiscesMinValue, RollDiscesMaxValue);
}

bool AKP_GameModeBase::CanPlaerRollDices(AKPPawn* PlayerPawn)
{
    return false;
}

void AKP_GameModeBase::RerollDices(AKPPawn* PlayerPawn)
{
    if (CanPlaerRollDices(PlayerPawn))
    {
        LastRollData.Value1 = RollDice();
        LastRollData.Value2 = RollDice();
        OnRerollDices.Broadcast(LastRollData, PlayerPawn);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Try reroll the dices, State is Lock"))
    }
}
