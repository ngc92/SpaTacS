// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipComponentBase.h"


// Sets default values for this component's properties
UShipComponentBase::UShipComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShipComponentBase::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UShipComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UShipComponentBase::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UShipComponentBase::GetMaximumHealth() const
{
	return MaximumHealth;
}

float UShipComponentBase::GetHealthStatus() const
{
	return CurrentHealth / MaximumHealth;
}
