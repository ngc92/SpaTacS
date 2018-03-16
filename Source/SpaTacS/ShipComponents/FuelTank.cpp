// Fill out your copyright notice in the Description page of Project Settings.

#include "FuelTank.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"


// Sets default values for this component's properties
UFuelTank::UFuelTank()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFuelTank::BeginPlay()
{
	Super::BeginPlay();

	DistributionSystem = GetOwner()->FindComponentByClass<UDistributionSystemComponent>();
}


// Called every frame
void UFuelTank::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DistributionSystem && CurrentFuel > PumpRate * DeltaTime)
	{
		DistributionSystem->OfferFuel(PumpRate * DeltaTime);
		CurrentFuel -= PumpRate * DeltaTime;
	}
}

float UFuelTank::GetCapacity() const {
	return Capacity;
}

float UFuelTank::GetCurrent() const
{
	return CurrentFuel;
}