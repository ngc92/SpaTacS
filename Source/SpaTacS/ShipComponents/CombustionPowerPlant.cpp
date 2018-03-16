// Fill out your copyright notice in the Description page of Project Settings.

#include "CombustionPowerPlant.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include <algorithm>


float UCombustionPowerPlant::ProduceEnergy(float DeltaTime)
{
    if (LocalFuelCache < 2 * FuelConsumption)
    {
        float missing = 2 * FuelConsumption - LocalFuelCache;
        LocalFuelCache += DistributionSystem->RequestFuel(missing);
    }

    float ActiveBurnRate = FuelConsumption * WorkLevel;
    float BurnedFuel = std::min(ActiveBurnRate * DeltaTime, LocalFuelCache);

    LocalFuelCache -= BurnedFuel;
    return BurnedFuel * PowerPerFuel;
}

float UCombustionPowerPlant::GetMaximumEnergyProduction() const
{
	return FuelConsumption * PowerPerFuel;
}