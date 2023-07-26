// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.h"
#include "Item.generated.h"

UCLASS()
class MOVEMENTTEST_API AItem : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Item")
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
		int32 AmountAddedOnPickup = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
		TSubclassOf<UInventoryItem> InventoryItem;

	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BluePrintCallable) void BeginInteraction(ATestCharacter* interactingCharacter);
	
	UPROPERTY(BlueprintReadWrite, Category = "Item")
		bool CanInteract = true;
};
