// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "BoardPiece.generated.h"


class ACell;

UCLASS(Blueprintable, BlueprintType, abstract)
class KP_MINIGAME_API ABoardPiece : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

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
	void SetNewCellId(int32 NewCellId);

	UFUNCTION(BlueprintCallable, Category = BoardMove)
	int32 GetCurrentCellId() const; 

	UFUNCTION(BlueprintCallable, Category = Player)
	FORCEINLINE int32 GetOwnPlayerId()const {return OwnPlayerId;}
	UFUNCTION(BlueprintCallable, Category = Player)
	void SetOwnPlayerData(int32 PlayerId, const FColor& PlayerColor);

	UFUNCTION(BlueprintCallable, Category = Input)
	void EnableSelectability(bool NewState);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void SetOwnPlayerDataBP();

	UFUNCTION()
	bool TrySelect();

protected:
	UPROPERTY(Transient)
	int32 CurrentCellId;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	int32 OwnPlayerId = -1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	FColor OwnPlayerColor;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	bool bCandSelected = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	bool bSelected = false;


};
