// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KP_Structs.h"

ACell* FBoardData::GetGellByIdChecked(const int32 Id) const
{
    // you can use a custom findFunction 
    if (Id < 0 || Id >= Cells.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Try get CellById %d, Num cells = %d"), Id, Cells.Num());
        check(false);
        return nullptr;
    }
    return Cells[Id];
}
