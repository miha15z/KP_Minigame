// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CellAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UCellAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementIn;
	ATTRIBUTE_ACCESSORS(UCellAttributeSet, MovementIn)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementOut;
	ATTRIBUTE_ACCESSORS(UCellAttributeSet, MovementOut)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMovementPointsOverride;
	ATTRIBUTE_ACCESSORS(UCellAttributeSet, MaxMovementPointsOverride)
};
