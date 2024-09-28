// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNavigationSystem.h"
#include <Cell.h>

void UBoardNavigationSystem::CalculateOrthogonalLength(ACell* Origin, ACell* Destination, int& Length)
{
	Length = abs(Destination->CoordX - Origin->CoordX) - abs(Destination->CoordY - Origin->CoordY);
	return;
}
