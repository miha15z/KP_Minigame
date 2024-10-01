// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardPiece.h"
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
	
}

// Called every frame
void ABoardPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
