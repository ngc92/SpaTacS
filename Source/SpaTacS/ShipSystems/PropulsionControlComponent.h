// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "PropulsionControlComponent.generated.h"


class UEngineComponent;
class UThrusterComponent;
class USpaceMovementComponent;


/*
Low level propulsion control class. Allows for targeting a given acceleration, angular velocity,
cruise control, rotation.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UPropulsionControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPropulsionControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetValues)
    FVector DesiredAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetValues)
    FVector DesiredAngularVelocity;

    /// Gets the currently produced acceleration in m/s².
    UFUNCTION(BlueprintPure)
    FVector GetCurrentAcceleration() const;

    // ----------------------------------------
    //    Linear Velocity Control Parameters
    // ----------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
	float MaximumAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetValues)
    FVector DesiredVelocity;

    /// If this is set to true the PropulsionSystem will regularly update target accelerations to achieve the velocity set in `DesiredVelocity`.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityControl)
    bool bControlVelocity;

    /// Do not try to accelerate further when ||Target-Current|| < Tolarance, makes stuff a lot more stable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityControl)
    float VelocityTolerance = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityControl)
    float ProportionalVelocityControl = 2.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityControl)
    float DifferentialVelocityControl = 2.0;

    // ----------------------------------------
    //       Position Control Parameters
    // ----------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetValues)
    FVector DesiredPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
    float MaximumSpeed = 100;

    /// If this is set to true the PropulsionSystem will regularly update target velocity to achieve the velocity set in `DesiredVelocity`.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PositionControl)
    bool bControlPosition = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PositionControl)
    float ProportionalPositionControl = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PositionControl)
    float DifferentialPositionControl = 5.0;

    // ----------------------------------------
    //        Angle Control Parameters
    // ----------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetValues)
    FRotator DesiredRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Limits)
    float MaximumAngularVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AngleControl)
    float ProportionalAngleControl = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AngleControl)
    float DifferentialAngleControl = 5.0;

    /// If this is set to true the PropulsionSystem will regularly update the target angular velocities to achieve the rotation given in `DesiredRotation`.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AngleControl)
    bool bControlRotation;

    /// If this is set to true the PropulsionSystem will regularly update the target rotation to optimize engine utilization for thrust.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AngleControl)
    bool bRotateToAlignThrust;
	
	float GetShipMass() const;

private:
	UPROPERTY()
	TArray<UEngineComponent*> Engines;

	UPROPERTY()
	TArray<UThrusterComponent*> Thrusters;

	UPROPERTY()
	USpaceMovementComponent* MovementComponent;

    FVector LastRotationError;
    FVector LastVelocityError;
    FVector LastPositionError;
    FVector SmoothedTargetAcceleration;
};
