// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardPiece.h"
#include "BoardPieceAttributeSet.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABoardPiece::ABoardPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ABoardPiece::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent)) {
		AttributeSet = AbilitySystemComponent->GetSet<UBoardPieceAttributeSet>();
	}
}

// Called every frame
void ABoardPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardPiece::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		TrySelect();
	}
}

void ABoardPiece::SetNewCellId(int32 NewCellId)
{
	CurrentCellId = NewCellId;
}

int32 ABoardPiece::GetCurrentCellId() const
{
	return CurrentCellId;
}

void ABoardPiece::SetOwnPlayerData(int32 PlayerId, const FColor& PlayerColor)
{
	OwnPlayerId = PlayerId;
	OwnPlayerColor = PlayerColor;

	// update visualisation
	SetOwnPlayerDataBP();
}

void ABoardPiece::EnableSelectability(bool NewState)
{
	bCandSelected = NewState;
	OnChengSelectability();
}

EBoardPiece ABoardPiece::GetBoardPieceType() const
{
	return BoardPieceType;
}

void ABoardPiece::ConfirmSelection()
{
	if (bSelected != true)
	{
		bSelected = true;
		OnChengSelection();
	}
}

void ABoardPiece::ResetSelection()
{
	if (bSelected != false)
	{
		bSelected = false;
		OnChengSelection();
	}
}

bool ABoardPiece::IsAlive() const
{
	//TO DO
	return bAlive;
}

bool ABoardPiece::Kill(ABoardPiece* OherPawn)
{
	if (IsAlive())
	{
		// to do
		bAlive = false;
		SetActorHiddenInGame(true);
		SetActorLocation(FVector(0.f, 0.f, -1000.f));
		return true;
	}
	return false;
}

void ABoardPiece::OnChengSelection_Implementation()
{
	// only test
	if (bSelected)
	{
		this->SetActorScale3D(FVector(1.3f));
	}
	else
	{
		this->SetActorScale3D(FVector(1.f));
	}
	// To do : update visualObject;
}

void ABoardPiece::OnChengSelectability_Implementation()
{
	static const FVector SelectabilityOffset = FVector(0.f, 0.f, 50.f);
	//Only Test
	if (bCandSelected)
	{
		this->SetActorLocation(GetActorLocation() + SelectabilityOffset);
	}
	else
	{
		this->SetActorLocation(GetActorLocation() - SelectabilityOffset);
	}

	// To do Selectability visualization
}

bool ABoardPiece::TrySelect()
{
	if (bCandSelected && !bSelected)
	{
		OnTrySelectBoardPiece.Broadcast(this);
	}
	return bSelected;
}
