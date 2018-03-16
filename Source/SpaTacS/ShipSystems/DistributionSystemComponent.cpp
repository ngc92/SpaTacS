// Fill out your copyright notice in the Description page of Project Settings.

#include "DistributionSystemComponent.h"
#include "ShipComponents/MagazineComponent.h"
#include "ShipComponents/FuelTank.h"
#include "ShipComponents/PowerPlant.h"
#include "GameFramework/Actor.h"
#include <cmath>


// Sets default values for this component's properties
UDistributionSystemComponent::UDistributionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    FuelCache = 0.f;

    AverageFuelInflux = 0.f;
    AverageFuelOutflux = 0.f;

    StepFuelInflux = 0.f;
    StepFuelOutflux = 0.f;

    FuelFullfillmentRatio = 1.f;

    AverageEnergyInflux = 0.f;
    AverageEnergyOutflux = 0.f;

    StepEnergyInflux = 0.f;
    StepEnergyOutflux = 0.f;

    EnergyFullfillmentRatio = 1.f;
}


// Called when the game starts
void UDistributionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

    GetOwner()->GetComponents(Magazines);
	GetOwner()->GetComponents(FuelTanks);
	GetOwner()->GetComponents(PowerPlants);
}


// Called every frame
void UDistributionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Fuel calculations
	float interpolation = std::exp(-DeltaTime);

	AverageFuelInflux = FMath::Lerp(AverageFuelInflux, StepFuelInflux / DeltaTime, interpolation);
	StepFuelInflux = 0.f;

	AverageFuelOutflux = FMath::Lerp(AverageFuelOutflux, StepFuelOutflux / DeltaTime, interpolation);
	StepFuelOutflux = 0.f;

	if (AverageFuelOutflux > AverageFuelInflux && AverageFuelOutflux > 0)
	{
		// Ratio * Out == In
        FuelFullfillmentRatio = (AverageFuelInflux + FuelCache/2) / AverageFuelOutflux;
	}
	else {
        FuelFullfillmentRatio = 1.f;
	}

    // Energy calculations
    AverageEnergyInflux = FMath::Lerp(AverageEnergyInflux, StepEnergyInflux / DeltaTime, interpolation);
    StepEnergyInflux = 0.f;

    AverageEnergyOutflux = FMath::Lerp(AverageEnergyOutflux, StepEnergyOutflux / DeltaTime, interpolation);
    StepEnergyOutflux = 0.f;

    if (AverageEnergyOutflux > 1e-6)
    {
        // Ratio * Out == In
        EnergyFullfillmentRatio = (AverageEnergyInflux + CachedEnergy) / AverageEnergyOutflux;
    }
    else {
        EnergyFullfillmentRatio = 100.f;
    }

    CachedEnergy = GatheredEnergy;
    GatheredEnergy = 0.f;
}


float UDistributionSystemComponent::OfferFuel(float Amount)
{
	if (!(Amount > 0)) return 0;

	StepFuelInflux += Amount;

	if (FuelCache < 100.f)
	{
		FuelCache += Amount;
		return Amount;
	}
	return 0.f;
}

float UDistributionSystemComponent::RequestFuel(float Amount)
{
	if (!(Amount > 0)) return 0;

	StepFuelOutflux += Amount;

	Amount = FuelFullfillmentRatio * Amount;

    if (Amount > FuelCache)
    {
        Amount = FuelCache;
    }
	FuelCache -= Amount;

	return Amount;
}

/// Offers `Amount` Nm of energy to be used. Returns the amount of power actually wanted.
float UDistributionSystemComponent::OfferPower(float Amount)
{
    if (!(Amount > 0)) return 0;

    StepEnergyInflux += Amount;
    GatheredEnergy += Amount;

    if (EnergyFullfillmentRatio > 1.0)
    {
        Amount /= EnergyFullfillmentRatio;
    }

    return Amount;
}

/// Requests `Amount` of Nm energy. Returns the actual amount of energy provided.
float UDistributionSystemComponent::RequestPower(float Amount)
{
    if (!(Amount > 0)) return 0;

    StepEnergyOutflux += Amount;

    if (EnergyFullfillmentRatio < 1.0)
    {
        Amount *= EnergyFullfillmentRatio;
    }

    if (Amount > CachedEnergy)
    {
        Amount = CachedEnergy;
    }

    CachedEnergy -= Amount;
    return Amount;
}

int UDistributionSystemComponent::RequestItem(const UClass* DesiredType, int Amount)
{
    if (!(Amount > 0)) return 0;

    int Found = 0;
    for (auto Magazine : Magazines)
    {
        int Here = Magazine->GetFromStorage(DesiredType, Amount);
        Found += Here;
        Amount -= Here;
        if (Amount <= 0)
        {
            break;
        }
    }

    return Found;
}

FFuelTankStatus UDistributionSystemComponent::GetFuelInfo() const
{
	float TotalFuelCapacity = 0.f;
	float CurrentFuelAmount = 0.f;
	for (auto Tank : FuelTanks)
	{
		TotalFuelCapacity += Tank->GetCapacity();
		CurrentFuelAmount += Tank->GetCurrent();
	}

	FFuelTankStatus Result;
	Result.Capacity = TotalFuelCapacity;
	Result.Current = CurrentFuelAmount;

	return Result;
}

FPowerSystemStatus UDistributionSystemComponent::GetPowerInfo() const
{
	float TotalProductionCapacity = 0.f;
	for (auto Power : PowerPlants)
	{
		TotalProductionCapacity += Power->GetMaximumEnergyProduction();
	}

	FPowerSystemStatus Result;
	Result.MaximumGeneration = TotalProductionCapacity;
	Result.CurrentConsumption = AverageEnergyInflux;
	Result.CurrentGeneration = AverageEnergyOutflux;
	return Result;
}