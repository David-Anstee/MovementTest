// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TestCharacter.generated.h"


UCLASS()
class MOVEMENTTEST_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement") class UCustomCharacterMovementComponent* CustomCharacterMovementComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory") class UInventoryComponent* InventoryComponent;

public:
	// Sets default values for this character's properties
	ATestCharacter(const FObjectInitializer& ObjectInitializer);

	//First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FP_Mesh;
	
	// FPS camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
		class UEquipmentItem* EquippedItem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bPressedCustomJump;

	virtual void Jump() override;
	virtual void StopJumping() override;

	FCollisionQueryParams GetIgnoreCharacterParams() const;

#pragma region health functions

	void RegenStamina(float deltaTime);

	UFUNCTION(BluePrintCallable) void ApplyDamage(float damage);						//Takes a (positive) float as damage, subtracts it from character's current health
	UFUNCTION(BluePrintCallable) void ApplyHealing(float healing);						//Takes a (positive) float as healing, adds it to character's current health
	
	UFUNCTION(BluePrintCallable) void ApplyTempHealthBonus(float healthModifier);
	UFUNCTION(BluePrintCallable) void ApplyTempHealthPenalty(float healthModifier);
	
	UFUNCTION(BluePrintCallable) void RevertTempHealthBonus();
	UFUNCTION(BluePrintCallable) void RevertTempHealthPenalty();
	
	UFUNCTION(BluePrintCallable) void SetMaxHealth(float newMaxHealth);					//Sets the character's (semi-permanent) base max health.
	UFUNCTION(BluePrintCallable) void SetCurrentHealth(int health);

	UFUNCTION(BluePrintCallable) void CalcEffectiveMaxHealth();							//Applies temporary modifier to effective max health.

#pragma endregion

#pragma region stamina functions


	UFUNCTION(BluePrintCallable) void ApplyStaminaDamage(float damage);						//Takes a (positive) float as damage, subtracts it from character's current health
	UFUNCTION(BluePrintCallable) void ApplyStaminaHealing(float healing);						//Takes a (positive) float as healing, adds it to character's current health

	UFUNCTION(BluePrintCallable) void ApplyTempStaminaBonus(float staminaModifier);
	UFUNCTION(BluePrintCallable) void ApplyTempStaminaPenalty(float staminaModifier);

	UFUNCTION(BluePrintCallable) void RevertTempStaminaBonus();
	UFUNCTION(BluePrintCallable) void RevertTempStaminaPenalty();

	UFUNCTION(BluePrintCallable) void SetMaxStamina(float newMaxStamina);					//Sets the character's (semi-permanent) base max health.
	UFUNCTION(BluePrintCallable) void SetCurrentStamina(int stamina);

	UFUNCTION(BluePrintCallable) void CalcEffectiveMaxStamina();							//Applies temporary modifier to effective max health.

#pragma endregion

#pragma region inventory functions

	UFUNCTION(BlueprintCallable, Category = "Items")
		void GiveItem(class UInventoryItem* Item, int32 amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void TakeItem(class UInventoryItem* Item, int32 amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void UseItem(class UInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void DropItem(class UInventoryItem* inventoryItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void UseHeldEquipment(class UEquipmentItem* equipment);

#pragma endregion

#pragma region variables

	UPROPERTY(BlueprintReadOnly) float MaxSideStrafeSpeed = 0.625;
	UPROPERTY(BlueprintReadOnly) float MaxBackwardsSpeed = 0.625;

	UPROPERTY(EditDefaultsOnly) int BaseMaxHealth = 100;					//Character's max health, before temporary modifiers
	UPROPERTY(BlueprintReadOnly) int TempHealthBonus = 0;					//Value of any temporary bonuses which increase max health
	UPROPERTY(BlueprintReadOnly) int TempHealthPenalty = 0;					//Value of any temporary penalties which decrease max health

	UPROPERTY(BlueprintReadOnly) int EffectiveMaxHealth = BaseMaxHealth;	//Character's max health, plus any temporary modifiers
	UPROPERTY(BlueprintReadOnly) int CurrentHealth = BaseMaxHealth;			//Character's current health

	UPROPERTY(EditDefaultsOnly) int BaseMaxStamina = 100;					//Character's max Stamina, before temporary modifiers
	UPROPERTY(BlueprintReadOnly) int TempStaminaBonus = 0;					//Value of any temporary bonuses which increase max Stamina
	UPROPERTY(BlueprintReadOnly) int TempStaminaPenalty = 0;					//Value of any temporary penalties which decrease max Stamina

	UPROPERTY(BlueprintReadOnly) int EffectiveMaxStamina = BaseMaxStamina;		//Character's max Stamina, plus any temporary modifiers
	UPROPERTY(BlueprintReadOnly) int CurrentStamina = BaseMaxStamina;			//Character's current Stamina

	UPROPERTY(BlueprintReadOnly) float StaminaRegenTimer = 0.f;
	UPROPERTY(BlueprintReadOnly) float ExhaustReoveryTimer = 0.f;
	UPROPERTY(BlueprintReadOnly) float IsExhausted = false;						//used for stamina; set to true when stamina runs out, the back to false after a few seconds
	UPROPERTY(BlueprintReadOnly) float SprintStaminaPause;

#pragma endregion

	FORCEINLINE UCustomCharacterMovementComponent* GetCustomCharacterMovement() const { return CustomCharacterMovementComponent; }

	void ChangeStrafeSpeed(float NewSideStrafeSpeed, float NewBackwardsSpeed);
};