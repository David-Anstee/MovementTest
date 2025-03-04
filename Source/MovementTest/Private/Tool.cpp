// Fill out your copyright notice in the Description page of Project Settings.

#include "Tool.h"

#include "TestCharacter.h"


// Sets default values
ATool::ATool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupItem = nullptr;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	if (VisualMesh)
	{
		VisualMesh->SetupAttachment(RootComponent);
	}

	//HurtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HurtBox"));

}

// Called when the game starts or when spawned
void ATool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATool::PickupTool(ATestCharacter* interactingCharacter)
{
	if (IsValid(interactingCharacter))
	{
		if (IsValid(PickupItem))
		{
			interactingCharacter->GiveItem(PickupItem);
			Destroy();
		}
	}
}

