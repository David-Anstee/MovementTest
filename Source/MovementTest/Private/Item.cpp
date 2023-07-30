// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "TestCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupItem = nullptr;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	if (VisualMesh)
	{
		VisualMesh->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::BeginInteraction(ATestCharacter* interactingCharacter, int32 amount = 1)
{
	if (IsValid(interactingCharacter))
	{
		if (IsValid(PickupItem))
		{
			interactingCharacter->GiveItem(PickupItem, amount);
			Destroy();
		}
	}
}

