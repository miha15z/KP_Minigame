// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"
#include "AbilitySystemComponent.h"
#include "CellAttributeSet.h"
#include "BoardPiece.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ACell::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		TrySelect();
	}
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent)) {
		AttributeSet = AbilitySystemComponent->GetSet<UCellAttributeSet>();
	}
}

void ACell::TrySelect()
{
	if (CanSelect())
	{
		SetState(ECellState::SelectToPlayer);
		OnSelectCell.Broadcast(this);
		UE_LOG(LogTemp, Warning, TEXT("Select Cell %d"), CellId);
	}
}

 bool ACell::CanSelect_Implementation() const
{
	return CurrentState == ECellState::SelectToNav;
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACell::Reset()
{
	SetState(ECellState::None);
}

void ACell::SetState(ECellState NewState)
{
	CurrentState = NewState;
	ACell::SetState_BP(NewState);
}

void ACell::PutPawnOnCell(ABoardPiece* Pawn, ABoardPiece** OutKilledPawn)
{
	check(Pawn);
	if (PawnPtr.IsValid() && PawnPtr.Get() != Pawn)
	{
		PawnPtr->Kill(Pawn);
		auto tempPawn = PawnPtr.Get();
		OutKilledPawn = &tempPawn;
		//To Do:: Kill 
	}

	PawnPtr = Pawn;
	ActivateOwnedAbilitiesOnStoodPawn();
	UE_LOG(LogTemp, Warning, TEXT("Put pawn %s to cell %d"), *Pawn->GetHumanReadableName(), CellId);
}

void ACell::LeavePawn(ABoardPiece* Pawn)
{
	// to do : reset cell abilities
	UE_LOG(LogTemp, Warning, TEXT(" pawn %s leave from cell %d"), *Pawn->GetHumanReadableName(), CellId);
	PawnPtr = nullptr;

}

ABoardPiece* ACell::GetStoodPawn() const
{
	return PawnPtr.Get();
}

void ACell::ActivateOwnedAbilitiesOnStoodPawn()
{
	ABoardPiece* StandingBoardPiece = PawnPtr.Get();
	if (StandingBoardPiece != NULL) {
		ActivateOwnedAbilities(StandingBoardPiece);
	}
}

void ACell::ActivateOwnedAbilities(ABoardPiece* TargetBoardPiece)
{
	for (UGameplayAbilityCellToPawnInfoHolder* AbilityInfoHolder : AbilityInfoHolders) {
		FGameplayAbilityCellToPawnInfo AbilityInfo = AbilityInfoHolder->GetInfo();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass, 1, INDEX_NONE, this);
		FGameplayEventData EventData; //= FGameplayEventData(this, TargetBoardPiece);
		EventData.Instigator = this;
		EventData.Target = TargetBoardPiece;
		EventData.OptionalObject = AbilityInfoHolder;
		GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(AbilitySpec, &EventData);
	}
}

