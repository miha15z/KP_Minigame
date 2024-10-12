// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CellAbilityDataAsset.generated.h"

class  UGameplayAbilityCellToPawnBase;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UCellAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = Cell)
	TSubclassOf<UGameplayAbilityCellToPawnBase> AbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = Cell)
	TSoftObjectPtr<UMaterialInterface> CellMaterial;

	UPROPERTY(EditDefaultsOnly, Category = Cell)
	FText Name;

	//description 
	UPROPERTY(EditDefaultsOnly, Category = Cell)
	FText Description;

public:
	UFUNCTION(BlueprintCallable, Category = Cell)
	UClass* GetAbilityClass() const;

	UFUNCTION(BlueprintCallable, Category = Cell)
	UMaterialInterface* GetCellMaterial() const;

	UFUNCTION(BlueprintPure, Category = Cell)
	const FText& GetName() const {return Name;}

	UFUNCTION(BlueprintPure, Category = Cell)
	const FText& GetDescription() const { return Description; }
};
