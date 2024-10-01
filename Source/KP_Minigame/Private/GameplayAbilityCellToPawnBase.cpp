// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityCellToPawnBase.h"

FGameplayAbilityCellToPawnInfo UGameplayAbilityCellToPawnBase::GetInfo() const
{
	FGameplayEventData EventData = CurrentEventData;
	const UGameplayAbilityCellToPawnInfoHolder* InfoHolder = Cast<UGameplayAbilityCellToPawnInfoHolder>(EventData.OptionalObject.Get());
	return InfoHolder->GetInfo();
}
