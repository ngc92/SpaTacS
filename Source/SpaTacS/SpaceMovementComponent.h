// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "SpaceMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Movement), meta = (BlueprintSpawnableComponent))
class SPATACS_API USpaceMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	USpaceMovementComponent();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	/**
	 * Adds the given amount of thrust to be applied for this frame.
	 * @param Thrust Thrust in Newtons.
	 */
	UFUNCTION(BlueprintCallable)
    void AddThrust(FVector Thrust);

	/**
	 * Adds the given amount of torque to be applied for this frame.
	 * @param Torque Torque in Nm.
	 */
	UFUNCTION(BlueprintCallable)
    void AddTorque(FVector Torque);

    void AddImpulseAtLocation(FVector Impulse, FVector WorldLocation);
    void AddImpulseAtCoM(FVector Impulse);


    UFUNCTION(BlueprintCallable)
    void SetVelocity(FVector NewVelocity);

	/// Gets the acceleration during the current/last frame in m/s²
    UFUNCTION(BlueprintPure)
    FVector GetAcceleration() const;
	/// Gets the angular velocity the current/last frame in °/s
    UFUNCTION(BlueprintPure)
    FVector GetAngularVelocity() const;

	/// Gets the mass of the updated component
	float GetMass() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Acceleration;

    FVector ForceBuffer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector AngularVelocity;

    FVector TorqueBuffer;

    FVector LastVelocity;
};
