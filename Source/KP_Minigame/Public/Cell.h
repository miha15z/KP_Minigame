// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Core/KP_Structs.h"
#include "Cell.generated.h"

class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCellChosenDelegate, ACell*, ChosenCell, int, PlayerID);

UENUM(BlueprintType)
enum class ECellState : uint8
{
	None,
	SelectToNav,
	SelectToPlayer,
	ShowPath
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class KP_MINIGAME_API ACell : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = CellInfo, BlueprintGetter = GetCellId, VisibleAnywhere)
	int32 CellId = 0;

	// descrete coordinates on a board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	FBoardCoord Coord;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	TArray<UGameplayAbility*> Abilities;

	// Populated automatically
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	TArray<ACell*> Neighbours;

	UPROPERTY(BlueprintAssignable)
	FOnCellChosenDelegate OnCellChosen;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UCellAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = CellInfo)
	ECellState CurrentState = ECellState::None;

public:
	// Sets default values for this actor's properties
	ACell();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	//UFUNCTION()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void Reset();

	UFUNCTION(BlueprintCallable, Category = Cells)
	void SetState(ECellState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = Cell)
	void SetState_BP(ECellState NewState);

	UFUNCTION(BlueprintCallable, Category = Cells)
	FORCEINLINE ECellState GetCurrentState() const
	{
		return CurrentState;
	}

	UFUNCTION(BlueprintCallable, Category = Cells)
	FORCEINLINE int32 GetCellId() const
	{
		return CellId;
	}
};
