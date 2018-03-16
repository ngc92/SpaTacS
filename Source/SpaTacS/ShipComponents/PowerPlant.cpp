// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerPlant.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UPowerPlant::UPowerPlant()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPowerPlant::BeginPlay()
{
	Super::BeginPlay();

    DistributionSystem = GetOwner()->FindComponentByClass<UDistributionSystemComponent>();
}


// Called every frame
void UPowerPlant::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    float ProducedEnergy = ProduceEnergy(DeltaTime);
    float WantedPower = DistributionSystem->OfferPower(ProducedEnergy);

    if (WantedPower >= 0.95 * ProducedEnergy) {
        WorkLevel += 0.1*DeltaTime;
    } else {
        WorkLevel -= 0.1*DeltaTime;
    }

    if (WorkLevel < 0.1f) {
        WorkLevel = 0.1f;
    } else if (WorkLevel > 1.f) {
        WorkLevel = 1.f;
    }
}

