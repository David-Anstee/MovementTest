#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


#pragma region custom movement modes enum

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None				UMETA(Hidden),
	CMOVE_Levitate			UMETA(DisplayName = "Levitate"),
	CMOVE_Glide				UMETA(DisplayName = "Glide"),
	CMOVE_MAX				UMETA(Hidden),
};

#pragma endregion

UCLASS()
class MOVEMENTTEST_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

#pragma region custom movement variables/parameters

	//Parameters
	UPROPERTY(EditDefaultsOnly) float Jog_MaxWalkSpeed = 500;					//Max speed while jogging (i.e. not sprinting or crouching); basically the 'base speed'

	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed = 0.625;				//Speed penalty while slowwalking, as a fraction of jog speed

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed = 1.3;				//Speed boost while sprinting, as a fraction of jog speed
	UPROPERTY(EditDefaultsOnly) float Crouch_MaxWalkSpeed = 0.625;				//Speed penalty while crouching, as a fraction of jog speed

	UPROPERTY(EditDefaultsOnly) float Walk_MaxSideStrafeSpeed = 0.625;			//Max left-right strafe speed while walking, as a fraction of jog speed
	UPROPERTY(EditDefaultsOnly) float Walk_MaxBackwardsSpeed = 0.625;			//Max backwards speed while walking, as a fraction of jog speed

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxSideStrafeSpeed = 0.625;		//Max left-right strafe speed while sprinting, as a fraction of sprint speed
	UPROPERTY(EditDefaultsOnly) float Sprint_MaxBackwardsSpeed = 0.625;			//Max backwards speed while sprinting, as a fraction of sprint speed


	UPROPERTY(EditDefaultsOnly) float Levitate_UpDownSpeed = 50.f;				//Speed for levitating up/down
	UPROPERTY(EditDefaultsOnly) float Levitate_AirSpeedMultiplier = 0.f;		//Multiplied by MaxSwimSpeed to get max speed for horizontal levitation


	float SprintDrainStaminaTimer;
	FVector GlideForwardVector;

	bool bWantsToSprint;				//Set to true when the 'sprint' button is pressed and false when released; allows the player to sprint
	bool bIsSprinting;

	bool bWantsToWalk;					//Toggled by the walk button; makes the player slowwalk
	bool bIsWalking;

	bool bWantsToLevitate;				//Toggled by the 'levitate' button; makes the player to enter the levitate custom movement mode when in the air
	bool LevitateUp;					//Set to true when the 'levitate up' button is pressed and false when released; makes a levitating player move upwards
	bool LevitateDown;					//Set to true when the 'levitate down' button is pressed and false when released; makes a levitating player move downwards

	bool bWantsToGlide;


#pragma endregion

#pragma region basic functions
public:
	UCustomCharacterMovementComponent();

protected:
	virtual void InitializeComponent() override;

public:
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	UPROPERTY(Transient, DuplicateTransient)
	class ATestCharacter* TestCharacterOwner;

#pragma endregion

#pragma region custom movement functions

private:															
	
	bool SprintIsValid();												//Checks if character can sprint currently
	void ApplySprint();													//Applies effects of sprint (speed boost, etc.)
	void RevertSprint();												//Reverts the effects of sprint

	bool IsValidWalk();
	void ApplyWalk();
	void RevertWalk();
	
	void EnterLevitate();												//Enters Levitation movement (CMove_Levitate); DOES NOT change levitation mode (bWantsToLevitate)
	void ExitLevitate();												//Exits Levitation movement (CMove_Levitate); DOES NOT change levitation mode (bWantsToLevitate)
	void PhysLevitate(float deltaTime, int32 Iterations);				//Controls the physics for the Levitation custom movement mode

	void EnterGlide();
	void ExitGlide();
	void PhysGlide(float deltaTime, int32 Iterations);

public:
	bool IsSprinting();
	bool IsWalking();
	void IncreaseSprintDrainStaminaTimer(float deltaTime);
	void DecreaseSprintDrainStaminaTimer(float deltaTime);
	void ResetSprintStaminaTimer();
	float getSprintStaminaTimer();

#pragma endregion

#pragma region blueprint functions

public:	
	UPROPERTY(BlueprintReadOnly) bool SprintDebugScreen;
	UPROPERTY(BlueprintReadOnly) float SprintStaminaTest = 0;

	UFUNCTION(BluePrintCallable) void SprintPressed();					//Should be called when the 'sprint' button is pressed
	UFUNCTION(BluePrintCallable) void SprintReleased();					//Should be called when the 'sprint' button is released
	UFUNCTION(BluePrintCallable) void SprintToggle();					//In testing

	UFUNCTION(BluePrintCallable) void WalkToggle();					//In testing

	UFUNCTION(BluePrintCallable) void CrouchPressed();					//Should be called when the 'crouch' button is pressed

	UFUNCTION(BluePrintCallable) void LevitatePressed();				//Should be called when the 'levitate' button is pressed

	UFUNCTION(BluePrintCallable) void LevitateUpPressed();				//Should be called when the 'levitate up' button is pressed
	UFUNCTION(BluePrintCallable) void LevitateUpReleased();				//Should be called when the 'levitate up' button is released
	UFUNCTION(BluePrintCallable) void LevitateDownPressed();			//Should be called when the 'levitate down' button is pressed
	UFUNCTION(BluePrintCallable) void LevitateDownReleased();			//Should be called when the 'levitate down' button is released

	UFUNCTION(BluePrintCallable) void GlidePressed();

	UFUNCTION(BluePrintCallable) void CustomDebug();

	UFUNCTION(BluePrintCallable) float GetWalk_MaxSideStrafeSpeed();
	UFUNCTION(BluePrintCallable) float GetWalk_MaxBackwardsSpeed();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

#pragma endregion

};
