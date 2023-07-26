// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOVEMENTTEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AddItem(class UItem* Item);
	bool RemoveItem(class UItem* Item);

	UFUNCTION(BluePrintCallable) void DebugPrintInventory();

	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class UInventoryItem*> DefaultItems;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TArray<class UInventoryItem*> InventoryItemList;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventoryUpdated OnInventoryUpdated;
};
