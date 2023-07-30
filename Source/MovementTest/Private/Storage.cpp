// Fill out your copyright notice in the Description page of Project Settings.


#include "Storage.h"
#include "InventoryComponent.h"
#include "TestCharacter.h"

// Sets default values
AStorage::AStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	if (VisualMesh)
	{
		VisualMesh->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AStorage::BeginPlay()
{
	Super::BeginPlay();

	if (!bDefaultInventoryFilled)
	{
		InventoryComponent->AddDefaultInventory();
		bDefaultInventoryFilled = true;
	}
}

// Called every frame
void AStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStorage::BeginInteraction(ATestCharacter* interactingCharacter)
{
	if (IsValid(interactingCharacter))
	{
		InventoryComponent->DebugPrintInventory();
	}
}

void AStorage::GiveItem(UInventoryItem* Item)
{
	InventoryComponent->AddItem(Item);
}

