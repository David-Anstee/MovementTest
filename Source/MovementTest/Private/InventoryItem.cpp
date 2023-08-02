// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

UInventoryItem::UInventoryItem()
{
	ItemName = FText::FromString("UNNAMED ITEM");
	ItemDescription = FText::FromString("NO ITEM DESCRIPTION");
	DefaultDropItem = nullptr;
	Weight = 0.f;
}