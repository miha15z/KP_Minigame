// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BoardPieceAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class KP_MINIGAME_API UBoardPieceAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementPointsModifier;
	ATTRIBUTE_ACCESSORS(UBoardPieceAttributeSet, MovementPointsModifier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementPointsMax;
	ATTRIBUTE_ACCESSORS(UBoardPieceAttributeSet, MovementPointsMax)

};
