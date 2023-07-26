#include "CustomPlayerCameraManager.h"

#include "TestCharacter.h"
#include "CustomCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ACustomPlayerCameraManager::ACustomPlayerCameraManager()
{
}

void ACustomPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float deltaTime)
{
	Super::UpdateViewTarget(OutVT, deltaTime);

	if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UCustomCharacterMovementComponent* CMC = TestCharacter->GetCustomCharacterMovement();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			CMC->CrouchedHalfHeight - TestCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (CMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + deltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;		
		}

		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - deltaTime, 0.f, CrouchBlendDuration);
		}

		if (CMC->IsSprinting())						//Lerps FOV up when the player is sprinting
		{
			ApplySprintFOV();
		}
		else										//Lerps FOV towards default FOV when the player is not sprinting
		{
			RevertSprintFOV();
		}

		if (CMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}

void ACustomPlayerCameraManager::ApplySprintFOV()
{
	float SprintTargetFOV = PlayerFOV * 1.15;						//Sets the target FOV for sprinting. PlayerFOV needs to be changed to reflect the fov set by the player; currently it is just set to 90.
	SprintFOV = FMath::Lerp(SprintFOV, SprintTargetFOV, 0.20);		//Smoothly transitions the FOV to the sprinting target
	SetFOV(SprintFOV);												//Locks the FOV to the new value
}

void ACustomPlayerCameraManager::RevertSprintFOV()
{
	if (SprintFOV == PlayerFOV)										//If the sprinting fov is already the same as the player's, unlocks FOV
	{
		UnlockFOV();
	}
	else															//Smoothly transitions the FOV back to the default player fov
	{
		SprintFOV = FMath::Lerp(SprintFOV, PlayerFOV, 0.10);
		SetFOV(SprintFOV);											//Locks the FOV to the new value
	}
}
