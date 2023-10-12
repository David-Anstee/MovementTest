// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "RangedWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTTEST_API URangedWeaponItem : public UWeaponItem
{
	GENERATED_BODY()
	
	virtual void UseEquipment(ATestCharacter* UserCharacter) override;

	void Fire(ATestCharacter* ShooterCharacter);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
};
