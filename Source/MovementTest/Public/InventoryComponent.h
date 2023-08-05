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

	UFUNCTION(BluePrintCallable) void DebugPrintInventory();

	UFUNCTION(BluePrintCallable) void AddItem(UInventoryItem* Item, int32 amount = 1);

	UFUNCTION(BluePrintCallable) void RemoveItem(UInventoryItem* Item, int32 amount = 1);

	//Removes item(s) from this inventory and adds the same item(s) to another
	UFUNCTION(BluePrintCallable) bool DonateItem(UInventoryComponent* RecipientInventory, UInventoryItem* Item, int32 amount = 1);

	//Adds item(s) to this inventory and removes the same item(s) from another
	UFUNCTION(BluePrintCallable) bool StealItem(UInventoryComponent* DonorInventory, UInventoryItem* Item, int32 amount = 1);

	//Adds item(s) to this inventory and removes the same item(s) from another
	UFUNCTION(BluePrintCallable) bool ExchangeItem(UInventoryComponent* TradingInventory, UInventoryItem* ItemGiven, UInventoryItem* ItemTaken, int32 amountGiven = 1, int32 amountTaken = 1);

	UFUNCTION(BluePrintCallable) void AddDefaultInventory();

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		TArray<class UInventoryItem*> InventoryItemList;

	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class UInventoryItem*> DefaultItems;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventoryUpdated OnInventoryUpdated;
};
