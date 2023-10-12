// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentItem.h"
#include "TestCharacter.h"

void UEquipmentItem::AttachEquipment(ATestCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and (TODO) has no equipment yet
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the Test Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
}

void UEquipmentItem::UseEquipment(ATestCharacter* UserCharacter)
{
}
