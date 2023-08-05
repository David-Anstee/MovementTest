// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class MOVEMENTTEST_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	UInventoryItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
		FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
		FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		TSubclassOf<AActor> DropItem = AActor::StaticClass();

	UPROPERTY()
		class UInventoryComponent* OwningInventory;

	virtual void Use(class ATestCharacter* Character) PURE_VIRTUAL(UInventoryItem, );

	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class ATestCharacter* Character);

};
