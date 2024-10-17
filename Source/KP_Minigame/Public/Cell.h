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
class UAbilitySystemComponent;
class UCellAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCellChosenDelegate, ACell*, ChosenCell, int, PlayerID);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectCell, ACell*);

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
	TArray<TObjectPtr<UGameplayAbilityCellToPawnInfoHolder> > AbilityInfoHolders;

	// Populated automatically
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	TArray<TObjectPtr<ACell>> Neighbours;

	UPROPERTY(BlueprintAssignable)
	FOnCellChosenDelegate OnCellChosen;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr <UCellConfigAsset> CellConfig;

	FOnSelectCell OnSelectCell;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr < const UCellAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base")
	ECellState CurrentState = ECellState::None;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base")
	TWeakObjectPtr<ABoardPiece> PawnPtr = nullptr;

	UPROPERTY(VisibleAnywhere,Category = Ability)
	TArray<FGameplayAbilityCellToPawnInfo> AbilitiesInfo;
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

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyMaterial(const UMaterialInterface* InMaterial);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void Reset();

	UFUNCTION(BlueprintCallable, Category = Cells)
	void SetState(const ECellState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = Cell)
	void SetState_BP(const ECellState NewState);

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

	//UFUNCTION(BlueprintCallable, Category = Cells)
	void PutPawnOnCell(ABoardPiece* Pawn, ABoardPiece ** OutKilledPawn);

	UFUNCTION(BlueprintCallable, Category = Cells)
	void LeavePawn(const ABoardPiece * Pawn);

	UFUNCTION(BlueprintCallable, Category = Cells)
	void ReplaceByPawn(ABoardPiece* Pawn);

	UFUNCTION(BlueprintCallable, Category = Cells)
	ABoardPiece* GetStoodPawn() const;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void ActivateOwnedAbilitiesOnStoodPawn() const;

	UFUNCTION(BlueprintCallable, Category = Cells)
	void ActivateOwnedAbilities(const ABoardPiece * TargetBoardPiece) const;

	
	UFUNCTION(BlueprintCallable, Category = Cells)
	void AddAbility(const FGameplayAbilityCellToPawnInfo& Info);

	UFUNCTION(BlueprintCallable, Category = Cells)
	void AddAbilities(const TArray<FGameplayAbilityCellToPawnInfo>& InAbilitiesInfo);
};
