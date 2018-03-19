// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrusterComponent.h"
#include "CombustionThrusterComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (ShipComponent), meta = (BlueprintSpawnableComponent), hideCategories=(Rendering, Replication, Physics, Collision))
class SPATACS_API UCombustionThrusterComponent : public UThrusterComponent
{
	GENERATED_BODY()

public:
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

    float GetMaximumDriftAcceleration() const override;
    float GetMaximumTorque() const override;

protected:
    FVector ProduceDrift(float DeltaTime, FVector Desired) override;
    FVector ProduceTorque(float DeltaTime, FVector Desired) override;

	// Drifting
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DriftThrustPerFuel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DriftFuelRate;

    /// The amount of energy needed for this engine to cause maximum drift.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DriftEnergyConsumption;

	// Rotating
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationPerFuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationFuelRate;

    /// The amount of energy needed for this engine to cause maximum rotation.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RotateEnergyConsumption;
private:
	float LocalFuelCache;
};
