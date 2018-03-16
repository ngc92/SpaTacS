// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponents/EngineComponent.h"
#include "CombustionEngineComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Spaceship), meta = (BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UCombustionEngineComponent : public UEngineComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCombustionEngineComponent();

	virtual float getMaximumThrust() const override;
protected:
	/// The amount of fuel that this engine can maximally burn.  [KG/S]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FuelBurnRate;

	/// The amount of thrust that this engine can generate per fuel.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ThrustPerFuel;

    /// The amount of energy needed for this engine to operate.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EnergyConsumption;

    /// The amount of power the engine needs even in idle state.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float IdlePowerDraw;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
    float ProduceThrust(float DeltaTime) override;

	float LocalFuelCache;
};
