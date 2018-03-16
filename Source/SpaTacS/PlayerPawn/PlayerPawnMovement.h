/**
 * Movement component updates position of associated PrimitiveComponent during its tick
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerPawnMovement.generated.h"

/**
 * PlayerPawnMovement is a movement  largely based on FloatingPawnMovement. It allows to disable time dialation 
 * (for spectator-like camera) and can be attached to a moving frame of reference. The resulting movement is then
 * a superposition of the linear movement of the frame of reference and the actual pawn movement.
 *
 * Normally the root component of the owning actor is moved, however another component may be selected (see SetUpdatedComponent()).
 * During swept (non-teleporting) movement only collision of UpdatedComponent is considered, attached components will teleport to the end location ignoring collision.
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class SPATACS_API UPlayerPawnMovement : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UPlayerPawnMovement(const FObjectInitializer& ObjectInitializer);

	//Begin UActorComponent Interface
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//End UActorComponent Interface

	//Begin UMovementComponent Interface
	float GetMaxSpeed() const override { return MaxSpeed; }

protected:
	bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
public:
	//End UMovementComponent Interface

	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraPawnMovement)
	float MaxSpeed;

	/** Acceleration applied by input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraPawnMovement)
	float Acceleration;

	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraPawnMovement)
	float Deceleration;

	/**
	 * Setting affecting extra force applied when changing direction, making turns have less drift and become more responsive.
	 * Velocity magnitude is not allowed to increase, that only happens due to normal acceleration. It may decrease with large direction changes.
	 * Larger values apply extra force to reach the target direction more quickly, while a zero value disables any extra turn force.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraPawnMovement, meta=(ClampMin="0", UIMin="0"))
	float TurningBoost;

	/**
	 * Velocity of the frame of reference the movement is to be considered relative to.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraPawnMovement, meta=(ClampMin="0", UIMin="0"))
	FVector ReferenceVelocity;

protected:

	/** Update Velocity based on input. Also applies gravity. */
	virtual void ApplyControlInputToVelocity(float DeltaTime);

	/** Prevent Pawn from leaving the world bounds (if that restriction is enabled in WorldSettings) */
	virtual bool LimitWorldBounds();

	/** Set to true when a position correction is applied. Used to avoid recalculating velocity when this occurs. */
	UPROPERTY(Transient)
	uint32 bPositionCorrected:1;

    /** If true, component moves at full speed no matter the time dilation. Default is false. */
	UPROPERTY()
	uint32 bIgnoreTimeDilation:1;
};

