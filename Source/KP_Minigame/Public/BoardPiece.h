// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Core/KP_Structs.h"
#include "BoardPiece.generated.h"


class UAbilitySystemComponent;
class ACell;
class UBoardPieceAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrySelectBoardPiece, ABoardPiece*)

UCLASS(Blueprintable, BlueprintType, abstract)
class KP_MINIGAME_API ABoardPiece : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const UBoardPieceAttributeSet* AttributeSet;

	// maybe to use data from PlayerDataAsset
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBoardCoord> MovementDirections;

public:	
	// Sets default values for this actor's properties
	ABoardPiece();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;
	
	UFUNCTION(BlueprintCallable, Category = BoardMove)
	void SetNewCellId(const int32 NewCellId);

	UFUNCTION(BlueprintCallable, Category = BoardMove)
	int32 GetCurrentCellId() const; 

	UFUNCTION(BlueprintCallable, Category = Player)
	FORCEINLINE int32 GetOwnPlayerId() const { return OwnPlayerId; }

	UFUNCTION(BlueprintCallable, Category = Player)
	void SetOwnPlayerData(const int32 PlayerId, const FColor& PlayerColor);

	UFUNCTION(BlueprintCallable, Category = Input)
	void EnableSelectability(const bool NewState);

	UFUNCTION(BlueprintCallable, Category = Player)
	EBoardPiece GetBoardPieceType() const;

	void ConfirmSelection();
	void ResetSelection();

	UFUNCTION(BlueprintCallable, Category = Player)
	bool IsAlive() const;

	bool Kill(const ABoardPiece * OherPawn);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void MoveToCell(const int32 CellId, const FVector& CellLocation);

	const TArray<FBoardCoord>& GetMovementDirections() const;
	void SetupTeamMovementDirectionMultiplier(const FBoardCoord& DirectionMultiplier);

	// Get movements points available yet this turn
	UFUNCTION(BlueprintCallable)
	int32 GetAvailableMovementPoints() const;

	// Reset the number of movement points at the end of the turn
	UFUNCTION(BlueprintCallable)
	void ResetAvailableMovementPoints();

	// Detract points from the available points when the board piece moves
	UFUNCTION(BlueprintCallable)
	void ConsumeMovementPoints(const int32 Points);

protected:
	bool bAlive = true; // to do: chenge to atributes

protected:
	// maybe make Selectaility interface for Cell and BoardPiece
	UFUNCTION(BlueprintNativeEvent)
	void OnChengSelection();
	virtual void OnChengSelection_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnChengSelectability();
	virtual void OnChengSelectability_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void SetOwnPlayerDataBP();

	UFUNCTION()
	bool TrySelect();



protected:
	UPROPERTY(Transient)
	int32 CurrentCellId = -1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	int32 OwnPlayerId = -1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	FColor OwnPlayerColor;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	bool bCandSelected = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	bool bSelected = false;
	UPROPERTY( EditDefaultsOnly)
	EBoardPiece BoardPieceType;

	// Movement Points already consumed by this board piece  this turn
	UPROPERTY(VisibleAnywhere)
	int32 MovementPointsConsumed = 0;
public:
	FOnTrySelectBoardPiece OnTrySelectBoardPiece;

};
