// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponents/PowerPlant.h"
#include "CombustionPowerPlant.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Spaceship), meta = (BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UCombustionPowerPlant : public UPowerPlant
{
	GENERATED_BODY()
public:
	float GetMaximumEnergyProduction() const override;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FuelConsumption;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PowerPerFuel;
private:
    float ProduceEnergy(float DeltaTime) override;

    float LocalFuelCache;
};
