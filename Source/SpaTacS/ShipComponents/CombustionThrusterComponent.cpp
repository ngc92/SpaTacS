// Fill out your copyright notice in the Description page of Project Settings.

#include "CombustionThrusterComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include <algorithm>
#include <cmath>


// Called every frame
void UCombustionThrusterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Grab new fuel
	if (LocalFuelCache < 2 * (DriftFuelRate + RotationFuelRate))
	{
		float missing = 2 * (DriftFuelRate + RotationFuelRate) - LocalFuelCache;
		LocalFuelCache += DistributionSystem->RequestFuel(missing);
	}
}

float UCombustionThrusterComponent::GetMaximumDriftAcceleration() const
{
    if(LocalFuelCache > 0.f) {
        return DriftFuelRate * DriftThrustPerFuel;
    } else {
        return 0.f;
    }
}


float UCombustionThrusterComponent::GetMaximumTorque() const
{
    if (LocalFuelCache > 0.f) {
        return RotationFuelRate * RotationPerFuel;
    }
    else {
        return 0.f;
    }
}

FVector UCombustionThrusterComponent::ProduceDrift(float DeltaTime, FVector Desired)
{
    // Drifting
    float desiredDriftSquared = Desired.SizeSquared();
    if (desiredDriftSquared > 1e-6) {
        float desiredDrift = std::sqrt(desiredDriftSquared);
        float maxDrift = DriftFuelRate * DriftThrustPerFuel;
        float level = std::min(1.f, desiredDrift / maxDrift);

        float required_energy = DriftEnergyConsumption * DeltaTime * (level + 0.01);
        float energy = DistributionSystem->RequestPower(required_energy);
        float energy_factor = energy / required_energy;

        float BurnedFuel = std::min(level * DriftFuelRate * DeltaTime * energy_factor, LocalFuelCache);
        LocalFuelCache -= BurnedFuel;
        return BurnedFuel * DriftThrustPerFuel / desiredDrift / DeltaTime * DesiredDriftThrust;
    }
    else
    {
        return { 0.f, 0.f, 0.f };
    }
}

FVector UCombustionThrusterComponent::ProduceTorque(float DeltaTime, FVector Desired)
{
    // Rotation
    float desiredRotation = Desired.Size();
    if (desiredRotation > 1e-6f)
    {
        float maxRotation = RotationFuelRate * RotationPerFuel;
        float level = std::min(1.f, desiredRotation / maxRotation);

        float required_energy = RotateEnergyConsumption * DeltaTime * (level + 0.01);
        float energy = DistributionSystem->RequestPower(required_energy);
        float energy_factor = energy / required_energy;

        float BurnedFuel = std::min(level * RotationFuelRate * DeltaTime * energy_factor, LocalFuelCache);
        LocalFuelCache -= BurnedFuel;

        return BurnedFuel * RotationPerFuel / desiredRotation / DeltaTime * Desired;
    }
    else
    {
        return FVector(0.f, 0.f, 0.f);
    }
}

