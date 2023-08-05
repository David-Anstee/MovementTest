// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

#include "InventoryItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UInventoryComponent::DebugPrintInventory()
{
	for (auto& Item : InventoryItemList)
	{
		FString DelMe = (Item->ItemID).ToString();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DelMe);
	}
}

void UInventoryComponent::AddItem(UInventoryItem* Item, int32 amount)
{
	for (int i = 0; i < amount; i++)
	{
		InventoryItemList.Add(Item);
	}
}

void UInventoryComponent::RemoveItem(UInventoryItem* Item, int32 amount)
{
	for (int i = 0; i < amount; i++)
	{
		InventoryItemList.RemoveSingle(Item);
	}
}

bool UInventoryComponent::DonateItem(UInventoryComponent* RecipientInventory, UInventoryItem* Item, int32 amount)
{
	bool confirmationCheck = false;
	if (IsValid(RecipientInventory))
	{
		RecipientInventory->AddItem(Item, amount);
		RemoveItem(Item, amount);
		confirmationCheck = true;
	}
	return confirmationCheck;
}

bool UInventoryComponent::StealItem(UInventoryComponent* DonorInventory, UInventoryItem* Item, int32 amount)
{
	bool confirmationCheck = false;
	if (IsValid(DonorInventory))
	{
		AddItem(Item, amount);
		DonorInventory->RemoveItem(Item, amount);
		confirmationCheck = true;
	}
	return confirmationCheck;
}

bool UInventoryComponent::ExchangeItem(UInventoryComponent* TradingInventory, UInventoryItem* ItemGiven, UInventoryItem* ItemTaken, int32 amountGiven, int32 amountTaken)
{
	bool confirmationCheck = false;
	if (IsValid(TradingInventory))
	{
		confirmationCheck = true;
		StealItem(TradingInventory, ItemTaken, amountTaken);
		DonateItem(TradingInventory, ItemGiven, amountGiven);
	}
	return confirmationCheck;
}

void UInventoryComponent::AddDefaultInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "DefInv Init");
	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}