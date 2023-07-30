// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storage.generated.h"

UCLASS()
class MOVEMENTTEST_API AStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

	UPROPERTY(VisibleAnywhere, Category = "Storage")
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Storage")
		FText ItemDisplayName;

	UPROPERTY(BlueprintReadWrite, Category = "Storage")
		bool CanInteract = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bDefaultInventoryFilled = false;	// Whether the default inventory items have been added yet

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Storage") 
		class UInventoryComponent* InventoryComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BluePrintCallable) 
		void BeginInteraction(ATestCharacter* interactingCharacter);
	UFUNCTION(BlueprintCallable, Category = "Items")
		void GiveItem(class UInventoryItem* Item);
};
