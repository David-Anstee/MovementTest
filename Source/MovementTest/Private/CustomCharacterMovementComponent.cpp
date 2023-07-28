#include "CustomCharacterMovementComponent.h"
#include "..\Public\CustomCharacterMovementComponent.h"
#include "TestCharacter.h"
#include "Math/Vector.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PhysicsVolume.h"

#pragma region basic functions

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

void UCustomCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TestCharacterOwner = Cast<ATestCharacter>(GetOwner());					
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (IsMovingOnGround())														//Toggles sprint on/off
	{
		if (!IsSprinting() && SprintIsValid())													//Sprint effects are applied if player can sprint
		{
			ApplySprint();
		}

		if (!IsWalking() && IsValidWalk())													//Sprint effects are applied if player can sprint
		{
			ApplyWalk();
		}
	}
	if (IsMovingOnGround() && bWantsToLevitate)									//Starts Levitation movement if in Levitation mode and trying to Levitate upwards
	{
		if (LevitateUp)
		{
			EnterLevitate();
		}
	}
	if ((IsSprinting() && !SprintIsValid()))																//If sprint is not valid then sprint effects are reverted
	{
		RevertSprint();
	}
	if (IsWalking() && !IsValidWalk())																	//If sprint is not valid then sprint effects are reverted
	{
		RevertWalk();
	}
}

void UCustomCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (bWantsToLevitate && IsFalling())														//Player should enter Levitation movement if in levitation Mode and falling
	{
		EnterLevitate();
	}

	if (!bWantsToLevitate && IsCustomMovementMode(CMOVE_Levitate))								//Player should exit Levitation movement (CMOVE_Levitate) if not in Levitation mode (bWantsToLevitate)
	{
		ExitLevitate();
	}

	if (bWantsToGlide && IsFalling())
	{
		EnterGlide();
	}
	if (!bWantsToGlide && IsCustomMovementMode(CMOVE_Glide))
	{
		ExitGlide();
	}

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UCustomCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)														//Will need a case for every custom movement mode
	{
	case CMOVE_Levitate:
		PhysLevitate(deltaTime, Iterations);
		break;
	case CMOVE_Glide:
		PhysGlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
	}
}

bool UCustomCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}


float UCustomCharacterMovementComponent::GetWalk_MaxSideStrafeSpeed()
{
	return Walk_MaxSideStrafeSpeed;
}

float UCustomCharacterMovementComponent::GetWalk_MaxBackwardsSpeed()
{
	return Walk_MaxBackwardsSpeed;
}

#pragma endregion

#pragma region walk functions

void UCustomCharacterMovementComponent::WalkToggle()											//In testing
{
	bWantsToWalk = !bWantsToWalk;
}

bool UCustomCharacterMovementComponent::IsValidWalk()											//In testing
{
	if (bWantsToWalk && !bWantsToSprint && !bWantsToCrouch)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void UCustomCharacterMovementComponent::ApplyWalk()											//In testing
{
	bIsWalking = true;

	MaxWalkSpeed = Jog_MaxWalkSpeed * Walk_MaxWalkSpeed;
}

bool UCustomCharacterMovementComponent::IsWalking()
{
	return bIsWalking;
}

void UCustomCharacterMovementComponent::RevertWalk()											//In testing
{
	bIsWalking = false;

	if (!IsSprinting()) {
		MaxWalkSpeed = Jog_MaxWalkSpeed;
	}
}

#pragma endregion

#pragma region sprint functions

void UCustomCharacterMovementComponent::SprintPressed()											//lets UpdateCharacterStateBeforeMovement() know it needs to toggle sprint ON. Called by pressing the 'sprint' key
{
	bWantsToSprint = true;
}

void UCustomCharacterMovementComponent::SprintReleased()										//lets UpdateCharacterStateBeforeMovement() know it needs to toggle sprint OFF. Called by releasing the 'sprint' key
{
	bWantsToSprint = false;
}

void UCustomCharacterMovementComponent::SprintToggle()											//In testing
{
	bWantsToSprint = !bWantsToSprint;
}

void UCustomCharacterMovementComponent::ApplySprint()											//Called when in Walking mode and sprinting is valid (i.e. SprintIsValid() = true)
{
	bIsSprinting = true;

	MaxWalkSpeed = Jog_MaxWalkSpeed * Sprint_MaxWalkSpeed;
	TestCharacterOwner->ChangeStrafeSpeed(Sprint_MaxSideStrafeSpeed, Sprint_MaxBackwardsSpeed);

	bWantsToWalk = false;
	bWantsToCrouch = false;
}

void UCustomCharacterMovementComponent::RevertSprint()											//Called when in Walking mode and sprinting is invalid (i.e. SprintIsValid() = false)
{
	bIsSprinting = false;

	MaxWalkSpeed = Jog_MaxWalkSpeed;
	TestCharacterOwner->ChangeStrafeSpeed(Walk_MaxSideStrafeSpeed, Walk_MaxBackwardsSpeed);
}

bool UCustomCharacterMovementComponent::IsSprinting()
{
	//SprintDebugScreen = bIsSprinting;
	return bIsSprinting;
}

void UCustomCharacterMovementComponent::IncreaseSprintDrainStaminaTimer(float deltaTime)
{
	SprintDrainStaminaTimer += deltaTime;
	SprintStaminaTest = SprintDrainStaminaTimer;
}

void UCustomCharacterMovementComponent::DecreaseSprintDrainStaminaTimer(float deltaTime)
{
	SprintDrainStaminaTimer -= deltaTime;
	SprintStaminaTest = SprintDrainStaminaTimer;
}

void UCustomCharacterMovementComponent::ResetSprintStaminaTimer()
{
	SprintDrainStaminaTimer = 0;
}

float UCustomCharacterMovementComponent::getSprintStaminaTimer()
{
	return SprintDrainStaminaTimer;
}

bool UCustomCharacterMovementComponent::SprintIsValid()											//Checks if player is trying to sprint (bWantsToSprint) and has forward velocity; if so, returns true
{
	//const FVector ForwardVector = TestCharacterOwner->GetActorForwardVector();
	const FVector TempAcceleration = GetCurrentAcceleration();
	const float AnyAcceleration = FMath::Abs(TempAcceleration.X) + FMath::Abs(TempAcceleration.Y);
	//const float ForwardVelocity = FVector::DotProduct(ForwardVector, TempVelocity);

	if (bWantsToSprint && AnyAcceleration > KINDA_SMALL_NUMBER && !TestCharacterOwner->IsExhausted)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma endregion

#pragma region crouch functions

void UCustomCharacterMovementComponent::CrouchPressed()											//Should be called by pressing the 'crouch' key; should toggle ON/OFF crouch (this is a default function, not sure where)
{
	bWantsToCrouch = !bWantsToCrouch;
}

#pragma endregion

#pragma region levitate functions

void UCustomCharacterMovementComponent::EnterLevitate()											//Called by UpdateCharacterStateBeforeMovement()
{
	MaxCustomMovementSpeed = MaxSwimSpeed * Levitate_AirSpeedMultiplier;						//Sets horizontal levitation speed (i.e. 'swimming' through air)
	SetMovementMode(MOVE_Custom, CMOVE_Levitate);
}

void UCustomCharacterMovementComponent::ExitLevitate()											//Called by UpdateCharacterStateBeforeMovement()
{
	LevitateUp = false;
	LevitateDown = false;
	MaxCustomMovementSpeed = 600.f;
	SetDefaultMovementMode();
}

void UCustomCharacterMovementComponent::LevitatePressed()										//Should be called by pressing the 'levitate' key; should toggle ON/OFF levitation (via UpdateCharacterStateBeforeMovement())
{
	bWantsToLevitate = !bWantsToLevitate;
}

void UCustomCharacterMovementComponent::LevitateUpPressed()										//Should be called by pressing the 'levitate up' key; should toggle ON levitating up (in PhysLevitate())
{
	if (bWantsToLevitate)
	{
		LevitateDown = false;
		LevitateUp = true;
	}
}

void UCustomCharacterMovementComponent::LevitateUpReleased()									//Should be called by releasing the 'levitate up' key; should toggle OFF levitating up (in PhysLevitate())
{
	if (bWantsToLevitate)
	{
		LevitateUp = false;
	}
}
void UCustomCharacterMovementComponent::LevitateDownPressed()									//Should be called by pressing the 'levitate down' key; should toggle ON levitating down (in PhysLevitate())
{
	if (bWantsToLevitate)
	{
		LevitateUp = false;
		LevitateDown = true;
	}
}

void UCustomCharacterMovementComponent::LevitateDownReleased()									//Should be called by releasing the 'levitate down' key; should toggle OFF levitating down (in PhysLevitate())
{
	if (bWantsToLevitate)
	{
		LevitateDown = false;
	}
}

void UCustomCharacterMovementComponent::PhysLevitate(float deltaTime, int32 Iterations)			//Controls physics for the Levitation custom movement mode
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}


	//RestorePreAdditiveRootMotionVelocity();

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())						//This block is taken from PhysFly
	{
		const float Friction = 0.5f * GetPhysicsVolume()->FluidFriction;						//Try adjusting the float value 
		CalcVelocity(deltaTime, Friction, true, GetMaxBrakingDeceleration());
	}

	//ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;																	//This line and everything above is 'boiler plate'

	if (!bWantsToLevitate)
	{
		return;
	}

	if (!LevitateUp && !LevitateDown)															//Smoothly transitions vertical (Z) velocity to 0 when not levitating up or down; makes the player float in air
	{
		Velocity.Z = FMath::Lerp(Velocity.Z, 0.f, 0.033);
		if (FMath::Abs(Velocity.Z) < 5)
		{
			Velocity.Z = 0;
		}
	}
	else if (LevitateUp)																		//Smoothly gives the player upwards (+Z) velocity when they want to levitate up; makes them float up
	{
		Velocity.Z = FMath::Lerp(Velocity.Z, Levitate_UpDownSpeed, 0.33);
	}
	else if (LevitateDown)																		//Smoothly gives the player downwards (-Z) velocity when they want to levitating down; makes them float down (LMAO)
	{
		Velocity.Z = FMath::Lerp(Velocity.Z, -1 * (Levitate_UpDownSpeed), 0.33);
	}

	FVector OldLocation = UpdatedComponent->GetComponentLocation();								//This block is unchanged from PhysFly
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)																			//This block is unchanged from PhysFly
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	if (Velocity.Z < KINDA_SMALL_NUMBER)														//Won't stop levitation when horizontal velocity is positive; allows players to begin levitating by jumping
	{
		FFindFloorResult FloorResult;
		FindFloor(UpdatedComponent->GetComponentLocation(), FloorResult, false, &Hit);
		if (FloorResult.IsWalkableFloor())														//Transitions the player back to walking if they touch a floor; they remain in 'levitate mode' (bWantsToLevitate=true), just not currently levitating (not CMove_Levitate)
		{
			ExitLevitate();
			return;
		}
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}

#pragma endregion

#pragma region glide functions

void UCustomCharacterMovementComponent::EnterGlide()
{
	GlideForwardVector = TestCharacterOwner->GetActorForwardVector();
	SetMovementMode(MOVE_Custom, CMOVE_Glide);
}

void UCustomCharacterMovementComponent::ExitGlide()
{
	SetDefaultMovementMode();
}

void UCustomCharacterMovementComponent::GlidePressed()
{
	bWantsToGlide = !bWantsToGlide;
}

void UCustomCharacterMovementComponent::PhysGlide(float deltaTime, int32 Iterations)			//Controls physics for the Glide custom movement mode
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	//RestorePreAdditiveRootMotionVelocity();

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())						//This block is taken from PhysFly()
	{
		const float Friction = 0.0f * GetPhysicsVolume()->FluidFriction;						//CHANGED: Friction 0.5f -> 0.0f
		CalcVelocity(deltaTime, Friction, true, GetMaxBrakingDeceleration());
	}

	//ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;																	//This line and everything above is 'boiler plate'

	float timeTick = GetSimulationTimeStep(deltaTime, Iterations);

	if (!bWantsToGlide)
	{
		return;
	}

	// Basically, when gliding gravity is reduced by lift; lift is increased by forward Velocity. Will have to rework; idea: when you enter gliding, your forward vector is locked 

	const float DefaultTerminalVelocity = 4000.f;														//By default, terminal velocity is 40m/s
	float Gravity = 980.f*deltaTime;

	float ForwardVelocity = FVector::DotProduct(GlideForwardVector, Velocity);
	ForwardVelocity = ForwardVelocity;

	float EffectiveTerminalVelocity = FMath::Max(0, DefaultTerminalVelocity - (2*ForwardVelocity));
	EffectiveTerminalVelocity = FMath::Max(EffectiveTerminalVelocity, DefaultTerminalVelocity);
	float AppliedGravity = Gravity * (EffectiveTerminalVelocity/DefaultTerminalVelocity);

	Velocity.Z -= Gravity;
	Velocity.Z = FMath::Max(Velocity.Z, -1 * EffectiveTerminalVelocity);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();								//This block is unchanged from PhysFly
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)																			//This block is unchanged from PhysFly
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	if (Velocity.Z < KINDA_SMALL_NUMBER)														//Won't stop gliding when horizontal velocity is positive; allows players to begin gliding by jumping
	{
		FFindFloorResult FloorResult;
		FindFloor(UpdatedComponent->GetComponentLocation(), FloorResult, false, &Hit);
		if (FloorResult.IsWalkableFloor())														//Transitions the player back to walking if they touch a floor; they remain in 'glide mode' (bWantsToLevitate=true), just not currently gliding (not CMove_Gliding)
		{
			ExitGlide();
			return;
		}
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}

#pragma endregion

#pragma region debug functions

void UCustomCharacterMovementComponent::CustomDebug()
{
	const FVector ForwardVector = TestCharacterOwner->GetActorForwardVector();
	//UE_LOG(LogTemp, Warning, TEXT("Forward Vector: %s"), *ForwardVector.ToString());
	Velocity += ForwardVector*3500;
}

#pragma endregion