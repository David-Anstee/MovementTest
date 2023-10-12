// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTTEST_API UEquipmentItem : public UInventoryItem
{
	GENERATED_BODY()

public:

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachEquipment(ATestCharacter* TargetCharacter);

	virtual void UseEquipment(ATestCharacter* UserCharacter);

private:
	/** The Character holding this equipment*/
	ATestCharacter* Character;
};
