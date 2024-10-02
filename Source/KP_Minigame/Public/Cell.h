// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Core/KP_Structs.h"
#include "GameplayAbilityCellToPawnBase.h"
#include "Cell.generated.h"

class UGameplayAbility;
class ABoardPiece;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCellChosenDelegate, ACell*, ChosenCell, int, PlayerID);

UCLASS(BlueprintType, Blueprintable)
class KP_MINIGAME_API UCellConfigAsset : public UDataAsset {
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FColor DefaultColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FColor PossibleMovementColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FColor PathColor;
};

UENUM(BlueprintType, Blueprintable)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	FGameplayAbilitySpec TestSpec;

	UPROPERTY(BlueprintReadOnly, Category = CellInfo, BlueprintGetter = GetCellId, VisibleAnywhere)
	int32 CellId = 0;

	// descrete coordinates on a board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	FBoardCoord Coord;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TArray<UGameplayAbilityCellToPawnInfoHolder*> AbilityInfoHolders;

	// Populated automatically
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	TArray<ACell*> Neighbours;

	UPROPERTY(BlueprintAssignable)
	FOnCellChosenDelegate OnCellChosen;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCellConfigAsset* CellConfig;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UCellAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base")
	ECellState CurrentState = ECellState::None;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base")
	TWeakObjectPtr<ABoardPiece> PawnPtr;
public:
	// Sets default values for this actor's properties
	ACell();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TrySelect();

	UFUNCTION(BlueprintNativeEvent)
	bool CanSelect() const;
	virtual bool CanSelect_Implementation() const;
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

	UFUNCTION(BlueprintCallable, Category = Cells)
	void PutPawnOnCell(ABoardPiece* Pawn);

	UFUNCTION(BlueprintCallable, Category = Cells)
	void LeavePawn(ABoardPiece* Pawn);

	UFUNCTION(BlueprintCallable, Category = Cells)
	ABoardPiece* GetStoodPawn() const;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void ActivateOwnedAbilitiesOnStoodPawn();

	UFUNCTION(BlueprintCallable, Category = Cells)
	void ActivateOwnedAbilities(ABoardPiece* TargetBoardPiece);
};
