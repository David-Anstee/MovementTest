// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

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

void UInventoryComponent::AddDefaultInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "DefInv Init");
	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}