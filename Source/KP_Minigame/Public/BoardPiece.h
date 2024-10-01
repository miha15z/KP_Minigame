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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UBoardPieceAttributeSet* AttributeSet;

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

	
	UFUNCTION(BlueprintCallable, Category = BoardMove)
	void SetNewCellId(int32 NewCellId);
	UFUNCTION(BlueprintCallable, Category = BoardMove)
	int32 GetCurrentCellId() const; 
protected:
	UPROPERTY(Transient)
	int32 CurrentCellId;
};
