// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "ConsumableInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTTEST_API UConsumableInventoryItem : public UInventoryItem
{
	GENERATED_BODY()
	

protected:

	virtual void Use(class ATestCharacter* Character) override;
};
