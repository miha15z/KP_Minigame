// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/StaticArray.h"
#include "AbilitySystemInterface.h"
#include "Cell.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCellChosenDelegate, ACell*, ChosenCell, int, PlayerID);

USTRUCT(BlueprintType)
struct FBoardCoord
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int y;
};

UCLASS()
class KP_MINIGAME_API ACell : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// descrete coordinates on a board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	FBoardCoord Coord;

	// TODO: make into attribute. For now mocks possible blocking attributes.
	UPROPERTY()
	bool bBlocked;

	UPROPERTY(BlueprintAssignable)
	FOnCellChosenDelegate OnCellChosen;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UCellAttributeSet* AttributeSet;

public:
	// Sets default values for this actor's properties
	ACell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override 
	{
		return AbilitySystemComponent;
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
