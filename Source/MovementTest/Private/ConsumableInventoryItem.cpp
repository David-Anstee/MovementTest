// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableInventoryItem.h"
#include "TestCharacter.h"

void UConsumableInventoryItem::Use(class ATestCharacter* Character)
{
	Character->ApplyHealing(10.f);
}