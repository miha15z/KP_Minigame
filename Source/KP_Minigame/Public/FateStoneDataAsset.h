// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FateStoneDataAsset.generated.h"

class UGameplayAbilityFateStone;
class UTexture;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KP_MINIGAME_API UFateStoneDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbilityFateStone> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> IconUI = nullptr;



	// to do, work radius

public:
	UFUNCTION(BlueprintCallable)
	UClass* GetGameplayAbilityClass() const;

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIconTexture() const;

	UFUNCTION(BlueprintCallable)
	const FText& GetName() const;

	UFUNCTION(BlueprintCallable)
	const FText& GetDescrption() const;
};
