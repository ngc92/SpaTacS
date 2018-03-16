// Fill out your copyright notice in the Description page of Project Settings.

#include "CombustionEngineComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include <algorithm>

UCombustionEngineComponent::UCombustionEngineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	FuelBurnRate = 100;
    EnergyConsumption = 100;
    IdlePowerDraw = 10;
}

void UCombustionEngineComponent::BeginPlay()
{
	Super::BeginPlay();
	LocalFuelCache = 0.f;
}

float UCombustionEngineComponent::ProduceThrust(float DeltaTime)
{
    if (LocalFuelCache < 2 * FuelBurnRate)
	{
		float missing = 2 * FuelBurnRate - LocalFuelCache;
		LocalFuelCache += DistributionSystem->RequestFuel(missing);
	}

    float required_energy = (ThrustLevel * EnergyConsumption + IdlePowerDraw) * DeltaTime;
    float energy = DistributionSystem->RequestPower(required_energy);
    // just to be save so we don't divide by zero
    if(required_energy < DeltaTime)
    {
        required_energy = DeltaTime;
    }
    float energy_factor = energy / required_energy;

	float ActiveBurnRate = FuelBurnRate * ThrustLevel * energy_factor;
	float BurnedFuel = std::min(ActiveBurnRate * DeltaTime, LocalFuelCache);

    LocalFuelCache -= BurnedFuel;
	return BurnedFuel * ThrustPerFuel / DeltaTime;
}


float UCombustionEngineComponent::getMaximumThrust() const
{
	if (LocalFuelCache > 0)
	{
		return FuelBurnRate * ThrustPerFuel;
	}
	return 0.f;
}
