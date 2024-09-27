// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CellActorBase.h"

// Sets default values
ACellActorBase::ACellActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACellActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACellActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

