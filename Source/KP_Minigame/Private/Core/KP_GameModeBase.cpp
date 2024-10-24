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
#include "FateStoneDataAsset.h"
#include "FateStonePlayerStoreComponent.h"


AKP_GameModeBase* AKP_GameModeBase::GetKPGameMode(const UObject* WorldContext)
{
    auto World = WorldContext->GetWorld();
    check(World);
    return World->GetAuthGameMode< AKP_GameModeBase>();
}

AKP_GameModeBase::AKP_GameModeBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
    AbilitySystemComponent = CreateDefaultSubobject<UKP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    WinTags.AddTag(KP_GameplayTags::GameplayEvent_ActivateWin);

    FateStoneStore = CreateDefaultSubobject<UFateStonePlayerStoreComponent>(TEXT("FateStoneStore"));
    //  need GMStones + Player0 + Player1...
    FateStoneStore->MaxStoreSize = 100;
}

void AKP_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    check(!GenDataAsset.IsNull())
    GenDataAsset.LoadSynchronous();
    check(!BoardGeneratorClass.IsNull());
    auto Generator = NewObject<UGameBoardGeneratorBase>(this, BoardGeneratorClass.LoadSynchronous());
    BoardData = Generator->GenerateGameBoard(GenDataAsset.Get(), this);

    check(!BoardNavSystemClass.IsNull())
    BoardNavSystem = NewObject<UBoardNavigationSystem>(this, BoardNavSystemClass.LoadSynchronous());
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
    check(BotPawnClass.LoadSynchronous())
    // Select first player
    auto World = GetWorld();
    check(World);
    auto PlayerPawn = World->GetFirstPlayerController()->GetPawn<AKPPawn>();
    check(PlayerPawn);
    PlayerPawn->PlayerId = 0;
    PlayerPawn->SetGameModePtr(this);
    for (auto& PlayerData : BoardData.PlayersData)
    {
        PlayerPawn->InitBoardPieces(PlayerData.Pawns);
    }
    
    // Setup Startup fate stones( only for test)
    PlayerPawn->InitFateStore(GenDataAsset.LoadSynchronous()->GetPlayerData(0).StartupFateStones);

    //Make GameQueue
    QueuePawns.Enqueue(PlayerPawn);
    Pawns.Add(PlayerPawn);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    const FTransform SpawnTransform;
    for (int32 i = 1; i < BoardData.PlayersData.Num(); ++i)
    {
        auto PawnAI = World->SpawnActor<AKPPawn>(BotPawnClass.Get(), SpawnTransform, SpawnParams);
        PawnAI->PlayerId = i;
        PawnAI->SetGameModePtr(this);
        // only for test or using advance bot
        PawnAI->InitFateStore(GenDataAsset->GetPlayerData(i).StartupFateStones);
        for (auto& PlayerData : BoardData.PlayersData)
        {
            PawnAI->InitBoardPieces(PlayerData.Pawns);
        }
        QueuePawns.Enqueue(PawnAI);
        Pawns.Add(PawnAI);
    }
    UpdateGameBoard();
    SelectNextPawn();
    //only test or add defaults stones 
    InitFateStore(GenDataAsset->GetGMFateStonesData());

	//ui
    if (auto UIClassPtr = UIClass.LoadSynchronous())
    {
        PlayerUI = CreateWidget(GetWorld(), UIClassPtr);
        PlayerUI->AddToViewport();
    }

	//show selectStoneUI
	if (UISelectStonesToGameClass.LoadSynchronous())
	{
		auto* SelectStoneUI = CreateWidget(GetWorld(), UISelectStonesToGameClass.Get());
		SelectStoneUI->AddToViewport();
	}

    // firstInitUI and other
    OnFinishStep.Broadcast();
}

void AKP_GameModeBase::UpdateGameBoard()
{
    ResetCells();

	// Decrease fate stone turn counter
	Cast<UKP_AbilitySystemComponent>(AbilitySystemComponent)->DecreaseActiveTurnBasedEffectsCounters();

    //  to do: update pawns, apply abilities, update steps counters 
    // 
    //for (auto& PlayerData : BoardData.PlayersData)
    //{
    //    for (auto& PawnInfo : PlayerData.Pawns)
    //    {
    //       // PawnInfo.Pawn->...
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
            BoardPiece.Pawn->ResetSelection();
            BoardPiece.Pawn->EnableSelectability(false);
        }
    }
}

void AKP_GameModeBase::BeginPlay()
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

void AKP_GameModeBase::SelectCellForCurrentPlayer(ACell* Cell)
{
    if (CurrentPawn.IsValid())
    {
        CurrentPawn->SelectCell(Cell);
    }
}

void AKP_GameModeBase::InitFateStore(const TArray<TSoftObjectPtr<UFateStoneDataAsset>>& InitData)
{
    FateStoneStore->Init(InitData);
}

UFateStonePlayerStoreComponent* AKP_GameModeBase::GetFateStoneStore() const
{
    return  FateStoneStore;
}

bool AKP_GameModeBase::TryCurrentPawnGiveFateStone(const int32 FateStoneId)
{
    if (CurrentPawn->CanGiveFateStone())
    {
        UFateStoneDataAsset* FateStoneData = FateStoneStore->GiveFateStone(FateStoneId);
        if (FateStoneData && CurrentPawn->TryAddFateStone(FateStoneData))
        {
            OnBonusTaken.Broadcast();
            return  true;
        }
    }
    return false;
}

bool AKP_GameModeBase::AddFateStoneData(UFateStoneDataAsset* Data)
{
    return  FateStoneStore->TryAddFateStone(Data);
}

bool AKP_GameModeBase::IsDrawingRandomFateStone() const
{
    return bDrawingRandomFateStone;
}

bool AKP_GameModeBase::CanGiveFateStone(const AKPPawn* Pawn)
{
    //to do
    return Pawn == CurrentPawn && FateStoneStore->Num() > 0 /*&& 1 stone 1 turn*/;
}

int32 AKP_GameModeBase::GetRandomFateStoneIndex() const
{
    int32 NumStones = FateStoneStore->Num();
    if (NumStones > 0)
    {
        return FMath::RandRange(0, NumStones - 1);
    }
    return -1;
}

const UFateStoneDataAsset* AKP_GameModeBase::GetFateStoneData(const int32 Index)
{
    return FateStoneStore->GetFateStone(Index);
}

const FRollDicesData& AKP_GameModeBase::GetLastRollData() const
{
    return LastRollData;
}

bool AKP_GameModeBase::IsBonusRollData() const
{
    return LastRollData.Value1 == LastRollData.Value2;
}

void AKP_GameModeBase::TryGiveBonus()
{
    
    if (IsDrawingRandomFateStone())
    {
        FGameplayEventData EventData;
        EventData.Target = CurrentPawn.Get();
        AbilitySystemComponent->HandleGameplayEvent(KP_GameplayTags::GameplayEvent_SelectRandomFateStone, &EventData);
    }
    else if(CanGiveFateStone(CurrentPawn.Get()))
    {
        ShowGiveBonusUI();
    }

    //to do :ActivateGameCue  or ability GameplayAbilityGMRollSameNumer
}

void AKP_GameModeBase::ShowGiveBonusUI() const 
{
    OnGiveBonus.Broadcast(CurrentPawn->PlayerId);
}

const TArray<TSoftObjectPtr<UFateStoneDataAsset>>& AKP_GameModeBase::GetStonesInGameForPlayer(const int32 PlayerId) const
{
    check(PlayerId >= 0 && PlayerId < BoardData.PlayersData.Num());
    return BoardData.PlayersData[PlayerId].FateStonesInGame;
}

TSoftObjectPtr<UFateStoneDataAsset> AKP_GameModeBase::RemoveFateStoneInGameForPlayer(const int32 IndexStone, const int32 PlayerId)
{
    check(PlayerId >= 0 && PlayerId < BoardData.PlayersData.Num());
    TSoftObjectPtr<UFateStoneDataAsset> Item = BoardData.PlayersData[PlayerId].FateStonesInGame[IndexStone];
    BoardData.PlayersData[PlayerId].FateStonesInGame.RemoveAt(IndexStone);
    return Item;
}

void AKP_GameModeBase::AddFateStoneInGameForPlayer(const TSoftObjectPtr<UFateStoneDataAsset>& FateStoneData, const int32 PlayerId)
{
    check(PlayerId >= 0 && PlayerId < BoardData.PlayersData.Num());
    BoardData.PlayersData[PlayerId].FateStonesInGame.Add(FateStoneData);
}

void AKP_GameModeBase::TakePlayersFateStones()
{
    for (const auto& PlayerData : BoardData.PlayersData)
    {
        InitFateStore(PlayerData.FateStonesInGame);
    }
}

int32 AKP_GameModeBase::RollDice() const
{
    return FMath::RandRange(RollDiscesMinValue, RollDiscesMaxValue);
}

void AKP_GameModeBase::RollDices()
{
	LastRollData.Value1 = RollDice();
	LastRollData.Value2 = RollDice();
}

bool AKP_GameModeBase::CanPlaerRollDices(const AKPPawn* PlayerPawn) const 
{
    return PlayerPawn == CurrentPawn ; //  to do
}

void AKP_GameModeBase::OnRollDices() const 
{
    OnRerollDices.Broadcast(LastRollData, CurrentPawn.Get());
}

ACell* AKP_GameModeBase::GetCellByID(const int32 Id) const
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

    // We make the GameplayEvent in a PlayerPawn
    //activate TurnEndAbility
    //check(AbilitySystemComponent);
    //FGameplayTagContainer TempTagConteiner;
    //TempTagConteiner.AddTag(KP_GameplayTags::Gameplay_Ability_EndTurn);
    //AbilitySystemComponent->TryActivateAbilitiesByTag(TempTagConteiner);

    if(not CheckWinState())
    {
        // Resets Board Pieces at the end of the turn
        ResetBoardPieces();
        UpdateGameBoard();
        SelectNextPawn();
        OnFinishStep.Broadcast();

        // may be used only TurnEndAbility
        //activate TurnStartAbility
        check(AbilitySystemComponent);
        FGameplayTagContainer TempStartTagConteiner;
        TempStartTagConteiner.AddTag(KP_GameplayTags::Gameplay_Ability_StartTurn);
        AbilitySystemComponent->TryActivateAbilitiesByTag(TempStartTagConteiner);
    }
    StepsCounter++;
    return true;
}

bool AKP_GameModeBase::CheckWinState() const
{
    if (IsWin())
    {
        check(CurrentPawn.IsValid());
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
    if(CurrentPawn.IsValid())
    {
        QueuePawns.Enqueue(CurrentPawn);
    }
    QueuePawns.Dequeue(CurrentPawn);
    check(CurrentPawn.IsValid());
    CurrentPawn->PreMakeStepData();
}

AKPPawn* AKP_GameModeBase::GetCurrentPawn() const
{
    return CurrentPawn.Get();
}

AKPPawn* AKP_GameModeBase::GetPlayerPawnById(const int32 Id) const 
{
    AKPPawn* Result = nullptr;

    for (auto& Pawn : Pawns)
    {
        if (Pawn->PlayerId == Id)
        {
            Result = Pawn.Get();
            break;
        }
    }

    return Result;
}

void AKP_GameModeBase::EnableSelectabilityForBoardPieces(const int32 OwnerPlayerId, const bool NewState, const EBoardPiece BoardPieceType)
{
    check(BoardData.PlayersData.Num() > OwnerPlayerId);
    // apply for all
    if (BoardPieceType == EBoardPiece::None)
    {
        for (auto& PawnsInfo : BoardData.PlayersData[OwnerPlayerId].Pawns)
        {
            ABoardPiece* BoardPiece = PawnsInfo.Pawn;
            if (IsValid(BoardPiece) && BoardPiece->IsAlive())
            {
                BoardPiece->EnableSelectability(NewState);
            }
        }
    }
    // apply for type
    else
    {
        for (auto& PawnsInfo : BoardData.PlayersData[OwnerPlayerId].Pawns)
        {
            ABoardPiece* BoardPiece = PawnsInfo.Pawn;
            if (IsValid(BoardPiece) && BoardPiece->IsAlive() && BoardPiece->GetBoardPieceType() == BoardPieceType)
            {
                BoardPiece->EnableSelectability(NewState);
            }
        }
    }
}

void AKP_GameModeBase::EnableSelectabilityForBoardPiecesForAllPlayer(const bool NewState, const EBoardPiece BoardPieceType)
{
    for (int32 i = 0; i < BoardData.PlayersData.Num(); ++i)
    {
        EnableSelectabilityForBoardPieces(i, NewState, BoardPieceType);
    }
}

void AKP_GameModeBase::EnableSelectabilityForBoardPiecesForOtherPlayers(const int32 PlayerId, const bool NewState, const EBoardPiece BoardPieceType)
{
    for (int32 i = 0; i < BoardData.PlayersData.Num(); ++i)
    {
        if (PlayerId == i)
        {
            continue;
        }
        EnableSelectabilityForBoardPieces(i, NewState, BoardPieceType);
    }
}

void AKP_GameModeBase::LeaveCell(const int32 CellId, const ABoardPiece * BoardPiece)
{
    BoardData.GetGellByIdChecked(CellId)->LeavePawn(BoardPiece);
}

const TArray<FKPPawnInfo>& AKP_GameModeBase::GetBoardPiecesForPlayer(const int32 PlayerId) const
{
    check(PlayerId >= 0 && PlayerId < BoardData.PlayersData.Num());
    return BoardData.PlayersData[PlayerId].Pawns;
}
