// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "CustomCharacterMovementComponent.h"
//#include "..\Public\TestCharacter.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "Item.h"
#include "EquipmentItem.h"

// Sets default values
ATestCharacter::ATestCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FirstPersonCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FirstPersonCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FP_Mesh != nullptr);

	// Only the owning player sees this mesh.
	FP_Mesh->SetOnlyOwnerSee(true);
	
	// Attach the FPS mesh to the FPS camera.
	FP_Mesh->SetupAttachment(FirstPersonCameraComponent);
	
	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FP_Mesh->bCastDynamicShadow = false;
	FP_Mesh->CastShadow = false;

	// Setup Custom Movement Component
	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());

	// Setup Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	EquippedItem = nullptr;

	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CustomCharacterMovementComponent->IsSprinting() && CustomCharacterMovementComponent->IsMovingOnGround())
	{
		SprintStaminaPause = 1.0;
		CustomCharacterMovementComponent->IncreaseSprintDrainStaminaTimer(DeltaTime);
		if (CustomCharacterMovementComponent->getSprintStaminaTimer() >= 0.5)
		{
			CustomCharacterMovementComponent->ResetSprintStaminaTimer();
			ApplyStaminaDamage(1);
		}
		if (CurrentStamina < 1)
		{
			IsExhausted = true;
		}
	}
	else if (!CustomCharacterMovementComponent->IsSprinting())
	{
		if (IsExhausted)
		{
			ExhaustReoveryTimer += DeltaTime;
			if (ExhaustReoveryTimer >= 5)
			{
				IsExhausted = false;
	
				ExhaustReoveryTimer = 0.f;
				SprintStaminaPause = 0.f;
			}
		}
	
		if (SprintStaminaPause > 0)
		{
			SprintStaminaPause -= DeltaTime;
	
			if (SprintStaminaPause < 0)
			{
				SprintStaminaPause = 0;
			}
		}
	
		if (!IsExhausted && SprintStaminaPause <= 0 && CustomCharacterMovementComponent->getSprintStaminaTimer() > 0)
		{
			CustomCharacterMovementComponent->DecreaseSprintDrainStaminaTimer(DeltaTime);
		}
	
		if (!IsExhausted && SprintStaminaPause <= 0 && CustomCharacterMovementComponent->getSprintStaminaTimer() <= 0 && CurrentStamina < EffectiveMaxStamina)
		{
			RegenStamina(DeltaTime);
		}
	}
}
// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::Jump()
{
	Super::Jump();

	bPressedCustomJump = true;

	bPressedJump = false;
}

void ATestCharacter::StopJumping()
{
	Super::StopJumping();

	bPressedCustomJump = false;
}

FCollisionQueryParams ATestCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;
	
	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);
	
	return Params;
}

void ATestCharacter::RegenStamina(float deltaTime)
{
	StaminaRegenTimer += deltaTime;
	if (StaminaRegenTimer >= 0.25)
	{
		StaminaRegenTimer = 0;
		ApplyStaminaHealing(1.0);
	}
}

#pragma region health functions

void ATestCharacter::ApplyDamage(float damage)
{
	damage = FMath::RoundHalfFromZero(damage);
	int appliedDamage = FMath::Max(static_cast<int>(damage), 0);

	CurrentHealth = FMath::Max(CurrentHealth - appliedDamage, 0);
	CalcEffectiveMaxHealth();
}

void ATestCharacter::ApplyHealing(float healing)
{
	healing = FMath::RoundHalfFromZero(healing);
	int appliedHealing = FMath::Max(static_cast<int>(healing), 0);

	CurrentHealth = FMath::Min(CurrentHealth + appliedHealing, EffectiveMaxHealth);
	CalcEffectiveMaxHealth();
}

void ATestCharacter::ApplyTempHealthBonus(float healthModifier)
{
	healthModifier = FMath::RoundHalfFromZero(healthModifier);
	int appliedhealthModifier = FMath::Max(static_cast<int>(healthModifier), 0);

	if (TempHealthBonus <= appliedhealthModifier)		//should always use the most powerful health bonus
	{
		TempHealthBonus = appliedhealthModifier;
	}

	CalcEffectiveMaxHealth();
}

void ATestCharacter::ApplyTempHealthPenalty(float healthModifier)
{
	healthModifier = FMath::RoundHalfFromZero(healthModifier);
	int appliedhealthModifier = FMath::Max(static_cast<int>(healthModifier), 0);

	if (TempHealthPenalty <= appliedhealthModifier)		//should always use the most severe health penalty
	{
		TempHealthPenalty = appliedhealthModifier;
	}

	CalcEffectiveMaxHealth();
}

void ATestCharacter::RevertTempHealthBonus()
{
	TempHealthBonus = 0;
	CalcEffectiveMaxHealth();
}

void ATestCharacter::RevertTempHealthPenalty()
{
	TempHealthPenalty = 0;
	CalcEffectiveMaxHealth();
}

void ATestCharacter::SetMaxHealth(float newMaxHealth)
{
	int oldMaxHealth = BaseMaxHealth;
	int oldEffectiveMaxHealth = EffectiveMaxHealth;

	newMaxHealth = FMath::RoundHalfFromZero(newMaxHealth);
	int appliedNewMaxHealth = (static_cast<int>(newMaxHealth));

	BaseMaxHealth = appliedNewMaxHealth;
	CalcEffectiveMaxHealth();
}

void ATestCharacter::SetCurrentHealth(int health)
{
	CurrentHealth = health;
	CalcEffectiveMaxHealth();
}

void ATestCharacter::CalcEffectiveMaxHealth()
{
	int oldEffectiveMaxHealth = EffectiveMaxHealth;
	int adjustedMaxHealth = BaseMaxHealth + TempHealthBonus - TempHealthPenalty;

	float minEffectiveHealth = BaseMaxHealth * 0.1;
	minEffectiveHealth = FMath::RoundHalfFromZero(minEffectiveHealth);

	EffectiveMaxHealth = FMath::Max(adjustedMaxHealth, static_cast<int>(minEffectiveHealth));	//Max() to make sure effective health is never less than 1


	//Adjusts current health so that percentage is the same (e.g. 50/100 -> 75/150, current health remains 50%)
	float checkOldHealth = static_cast<float>(oldEffectiveMaxHealth);
	float checkCurrentHealth = static_cast<float>(CurrentHealth);
	float checkMaxHealth = static_cast<float>(EffectiveMaxHealth);
	float newHealth = (checkCurrentHealth / checkOldHealth) * checkMaxHealth;
	newHealth = FMath::RoundHalfFromZero(newHealth);
	CurrentHealth = FMath::Max(static_cast<int>(newHealth), 0);

}

#pragma endregion

#pragma region stamina functions

void ATestCharacter::ApplyStaminaDamage(float damage)
{
	damage = FMath::RoundHalfFromZero(damage);
	int appliedDamage = FMath::Max(static_cast<int>(damage), 0);

	CurrentStamina = FMath::Max(CurrentStamina - appliedDamage, 0);
	CalcEffectiveMaxStamina();
}

void ATestCharacter::ApplyStaminaHealing(float healing)
{
	healing = FMath::RoundHalfFromZero(healing);
	int appliedHealing = FMath::Max(static_cast<int>(healing), 0);

	CurrentStamina = FMath::Min(CurrentStamina + appliedHealing, EffectiveMaxStamina);
	CalcEffectiveMaxStamina();
}

void ATestCharacter::ApplyTempStaminaBonus(float staminaModifier)
{
	staminaModifier = FMath::RoundHalfFromZero(staminaModifier);
	int appliedstaminaModifier = FMath::Max(static_cast<int>(staminaModifier), 0);

	if (TempStaminaBonus <= appliedstaminaModifier)		//should always use the most powerful Stamina bonus
	{
		TempStaminaBonus = appliedstaminaModifier;
	}

	CalcEffectiveMaxStamina();
}

void ATestCharacter::ApplyTempStaminaPenalty(float staminaModifier)
{
	staminaModifier = FMath::RoundHalfFromZero(staminaModifier);
	int appliedstaminaModifier = FMath::Max(static_cast<int>(staminaModifier), 0);

	if (TempStaminaPenalty <= appliedstaminaModifier)		//should always use the most severe Stamina penalty
	{
		TempStaminaPenalty = appliedstaminaModifier;
	}

	CalcEffectiveMaxStamina();
}

void ATestCharacter::RevertTempStaminaBonus()
{
	TempStaminaBonus = 0;
	CalcEffectiveMaxStamina();
}

void ATestCharacter::RevertTempStaminaPenalty()
{
	TempStaminaPenalty = 0;
	CalcEffectiveMaxStamina();
}

void ATestCharacter::SetMaxStamina(float newMaxStamina)
{
	int oldMaxStamina = BaseMaxStamina;
	int oldEffectiveMaxStamina = EffectiveMaxStamina;

	newMaxStamina = FMath::RoundHalfFromZero(newMaxStamina);
	int appliedNewMaxStamina = (static_cast<int>(newMaxStamina));

	BaseMaxStamina = appliedNewMaxStamina;
	CalcEffectiveMaxStamina();
}

void ATestCharacter::SetCurrentStamina(int stamina)
{
	CurrentStamina = stamina;
	CalcEffectiveMaxStamina();
}

void ATestCharacter::CalcEffectiveMaxStamina()
{
	int oldEffectiveMaxStamina = EffectiveMaxStamina;
	int adjustedMaxStamina = BaseMaxStamina + TempStaminaBonus - TempStaminaPenalty;

	float minEffectiveStamina = BaseMaxStamina * 0.1;
	minEffectiveStamina = FMath::RoundHalfFromZero(minEffectiveStamina);

	EffectiveMaxStamina = FMath::Max(adjustedMaxStamina, static_cast<int>(minEffectiveStamina));	//Max() to make sure effective Stamina is never less than 1


	//Adjusts current Stamina so that percentage is the same (e.g. 50/100 -> 75/150, current Stamina remains 50%)
	float checkOldStamina = static_cast<float>(oldEffectiveMaxStamina);
	float checkCurrentStamina = static_cast<float>(CurrentStamina);
	float checkMaxStamina = static_cast<float>(EffectiveMaxStamina);
	float newStamina = (checkCurrentStamina / checkOldStamina) * checkMaxStamina;
	newStamina = FMath::RoundHalfFromZero(newStamina);
	CurrentStamina = FMath::Max(static_cast<int>(newStamina), 0);
}


#pragma endregion

#pragma region inventory functions


void ATestCharacter::GiveItem(UInventoryItem* Item, int32 amount)
{
	InventoryComponent->AddItem(Item, amount);
}

void ATestCharacter::TakeItem(UInventoryItem* Item, int32 amount)
{
	InventoryComponent->RemoveItem(Item, amount);
}

void ATestCharacter::UseItem(UInventoryItem* Item)
{
	if (IsValid(Item))
	{
		Item->Use(this);
		Item->OnUse(this);
	}
}

void ATestCharacter::DropItem(UInventoryItem* inventoryItem)
{
	UCameraComponent* camera = FirstPersonCameraComponent;

	FVector DropLocation;
	FHitResult Hit;

	FVector TraceStart;
	FVector TraceEnd;

	if (IsValid(camera))
	{
		TraceStart = camera->GetComponentLocation();
		TraceEnd = TraceStart + (camera->GetComponentRotation().Vector() * 187.5f);
	}
	else
	{
		TraceStart = GetActorLocation();
		TraceEnd = TraceStart + (GetActorForwardVector() * 187.5f);
	}

	ECollisionChannel DropCollisionChannel = ECC_Visibility;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, DropCollisionChannel, QueryParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

	if (!Hit.bBlockingHit)
	{
		DropLocation = TraceEnd;
	}
	else if (Hit.bBlockingHit)
	{
		DropLocation = Hit.ImpactPoint;
	}

	UWorld* thisWorld = GetWorld();
	FRotator targetRotation = GetActorRotation();

	TSubclassOf<AActor> spawnedItem = inventoryItem->DropItem;
	
	AActor* SpawnedActor = thisWorld->SpawnActor<AActor>(spawnedItem, DropLocation, targetRotation);
	
	if (IsValid(SpawnedActor))
	{
		TakeItem(inventoryItem);
	}
}

void ATestCharacter::UseHeldEquipment(UEquipmentItem* equipment)
{
	if (IsValid(equipment))
	{
		equipment->UseEquipment(this);
		equipment->OnUse(this);
	}
}

//if (IsValid(inventoryItem) && IsValid(inventoryItem->DropItem))
//{
//	UWorld* thisWorld = GetWorld();
//	FVector targetLocation = GetActorLocation() + 150 * GetActorForwardVector();
//	FRotator targetRotation = GetActorRotation();
//
//	TSubclassOf<AActor> spawnedItem = inventoryItem->DropItem;
//
//	AActor* SpawnedActor = thisWorld->SpawnActor<AActor>(spawnedItem, targetLocation, targetRotation);
//
//	if (IsValid(SpawnedActor))
//	{
//		TakeItem(inventoryItem);
//	}
//}

#pragma endregion

void ATestCharacter::ChangeStrafeSpeed(float NewSideStrafeSpeed, float NewBackwardsSpeed)		//Called in CustomCharacterMovementComponent
{
	MaxSideStrafeSpeed = NewSideStrafeSpeed;
	MaxBackwardsSpeed = NewBackwardsSpeed;
}