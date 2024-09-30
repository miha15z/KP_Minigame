// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KPPawnBase.h"

// Sets default values
AKPPawnBase::AKPPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKPPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKPPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKPPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

