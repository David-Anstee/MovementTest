#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CustomPlayerCameraManager.generated.h"

UCLASS()
class MOVEMENTTEST_API ACustomPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration = .15f;
	float CrouchBlendTime;

public:
	ACustomPlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float deltaTime) override;

	void ApplySprintFOV();								//Called in UpdateViewTarget; transitions FOV to a higher target when the player is sprinting
	void RevertSprintFOV();								//Called in UpdateViewTarget; transitions FOV to the default when the player is not sprinting
	float PlayerFOV = GetFOVAngle();					//Should be equal to the FOV set by the player; not currently the case (Will always be 90)
	float SprintFOV = PlayerFOV;						//SprintFOV is the value used when shifting FOV because of sprinting; constantly updated so that transitions are smooth
};

