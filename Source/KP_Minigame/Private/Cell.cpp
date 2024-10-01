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

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent)) {
		AttributeSet = AbilitySystemComponent->GetSet<UCellAttributeSet>();
	}
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACell::Reset()
{
}

void ACell::SetState(ECellState NewState)
{
	CurrentState = NewState;
	ACell::SetState_BP(NewState);
}

void ACell::StoodPawnOnCell(ABoardPiece* Pawn)
{
	PawnPtr = Pawn;
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
	for (TSubclassOf<UGameplayAbilityCellToPawnBase>& Ability : StartupAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this);
		FGameplayEventData EventData; //= FGameplayEventData(this, TargetBoardPiece);
		EventData.Instigator = this;
		EventData.Target = TargetBoardPiece;
		GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(AbilitySpec, &EventData);
	}

}

