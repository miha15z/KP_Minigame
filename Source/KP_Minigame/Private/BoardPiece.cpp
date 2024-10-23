// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardPiece.h"
#include "BoardPieceAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "KP_AbilitySystemComponent.h"

// Sets default values
ABoardPiece::ABoardPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UKP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ABoardPiece::BeginPlay()
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

	AttributeSet = AbilitySystemComponent->GetSet<UBoardPieceAttributeSet>();


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

void ABoardPiece::SetNewCellId(const int32 NewCellId)
{
	CurrentCellId = NewCellId;
}

int32 ABoardPiece::GetCurrentCellId() const
{
	return CurrentCellId;
}

void ABoardPiece::SetOwnPlayerData(const int32 PlayerId, const FColor& PlayerColor)
{
	OwnPlayerId = PlayerId;
	OwnPlayerColor = PlayerColor;

	// update visualization
	SetOwnPlayerDataBP();
}

void ABoardPiece::EnableSelectability(const bool NewState)
{
	if (NewState != bCandSelected)
	{
		bCandSelected = NewState;
		OnChengSelectability();
	}
}

EBoardPiece ABoardPiece::GetBoardPieceType() const
{
	return BoardPieceType;
}

void ABoardPiece::CopyState(const ABoardPiece* Other)
{
	// Copy the identifying parameters
	CurrentCellId = Other->CurrentCellId;
	SetOwnPlayerData(Other->OwnPlayerId, Other->OwnPlayerColor);
	bCandSelected = Other->bCandSelected;
	//bSelected = Other->bSelected;
	TeamMovementMultiplier = Other->TeamMovementMultiplier;
	SetupTeamMovementDirectionMultiplier(TeamMovementMultiplier);
	MovementPointsConsumed = Other->MovementPointsConsumed;
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

bool ABoardPiece::Kill(const ABoardPiece * Killer)
{
	if (IsAlive())
	{
		// to do
		bAlive = false;
		SetActorHiddenInGame(true);
		SetActorLocation(FVector(0.f, 0.f, -1000.f));
		check(Killer);
		UE_LOG(LogTemp, Display, TEXT("Player's (%d) the %s kill the %s (player's (%d))"), 
			GetOwnPlayerId(), 
			*GetHumanReadableName(),
			*Killer->GetHumanReadableName(),
			Killer->GetOwnPlayerId())
		return true;
	}
	return false;
}

bool ABoardPiece::ForceKill()
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

void ABoardPiece::MoveToCell(const int32 CellId, const FVector& CellLocation)
{
	CurrentCellId = CellId;
	//SetActorLocation(CellLocation);
}

const TArray<FBoardCoord>& ABoardPiece::GetMovementDirections() const
{
	return MovementDirections;
}

void ABoardPiece::SetupTeamMovementDirectionMultiplier(const FBoardCoord& DirectionMultiplier)
{
	TeamMovementMultiplier = DirectionMultiplier;
	// TODO
	// normalize DirectionMultiplier

	// modify movement direction according to the team multiplier
	for (FBoardCoord& Direction : MovementDirections) 
	{
		Direction = DirectionMultiplier * Direction;
	}
}

int32 ABoardPiece::GetAvailableMovementPoints() const
{
	return FMath::FloorToInt32(GetAbilitySystemComponent()->GetNumericAttribute(UBoardPieceAttributeSet::GetMovementPointsMaxAttribute())) - MovementPointsConsumed;
}

void ABoardPiece::ResetAvailableMovementPoints()
{
	MovementPointsConsumed = 0;
}

void ABoardPiece::ConsumeMovementPoints(const int32 Points)
{
	if (GetAvailableMovementPoints() >= Points)
	{
		MovementPointsConsumed += Points;
	}
	else 
	{
		check(false);
	}
	
}

void ABoardPiece::OnChengSelection_Implementation()
{
	// only test
	if (bSelected)
	{
		this->SetActorScale3D(FVector(1.4f));
	}
	else
	{
		this->SetActorScale3D(FVector(1.2f));
	}
	// To do : update visualObject;
}

void ABoardPiece::OnChengSelectability_Implementation()
{
	static const FVector SelectabilityOffset = FVector(0.f, 0.f, 50.f);
	//Only Test
	if (bCandSelected)
	{
		this->SetActorScale3D(FVector(1.2f));
		this->SetActorEnableCollision(true);
	}
	else
	{
		this->SetActorScale3D(FVector(1.f));
		// disable collision
		this->SetActorEnableCollision(false);
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
