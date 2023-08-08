// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.h"

#include "Tool.generated.h"

UCLASS()
class MOVEMENTTEST_API ATool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Item")
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText ToolDisplayName;

	UPROPERTY(Instanced, EditDefaultsOnly, Category = "Item")
		UInventoryItem* PickupItem;

//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hurt Box") 
//		class UCapsuleComponent* HurtBox;

	UFUNCTION(BluePrintCallable) void
		PickupTool(ATestCharacter* interactingCharacter);

};
