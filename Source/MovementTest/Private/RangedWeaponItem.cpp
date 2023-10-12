// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeaponItem.h"
#include "TestCharacter.h"

void URangedWeaponItem::UseEquipment(ATestCharacter* UserCharacter)
{
	Fire(UserCharacter);
}

void URangedWeaponItem::Fire(ATestCharacter* ShooterCharacter)
{
	//Get Character's Camera transform
	FVector CameraLocation;
	FRotator CameraRotation;

	ShooterCharacter->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
	
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	UCameraComponent* Camera = ShooterCharacter->FirstPersonCameraComponent;

	FVector HitLocation;
	FHitResult Hit;

	FVector TraceStart;
	FVector TraceEnd;

	if (IsValid(Camera))
	{
		TraceStart = Camera->GetComponentLocation();
		TraceEnd = TraceStart + (Camera->GetComponentRotation().Vector() * 100000.f);
	}

	ECollisionChannel DropCollisionChannel = ECC_Visibility;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ShooterCharacter);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, DropCollisionChannel, QueryParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

	if (!Hit.bBlockingHit)
	{
		HitLocation = TraceEnd;
	}
	else if (Hit.bBlockingHit)
	{
		HitLocation = Hit.ImpactPoint;
		Hit.GetActor();
	}



}