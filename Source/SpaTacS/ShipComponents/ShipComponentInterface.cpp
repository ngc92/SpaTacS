// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipComponentInterface.h"


float IShipComponentInterface::GetCurrentHealth() const
{
	return GetHealthData().CurrentHealth;
}

float IShipComponentInterface::GetMaximumHealth() const
{
	return GetHealthData().MaximumHealth;
}


float IShipComponentInterface::GetHealthStatus() const
{
	auto Health = GetHealthData();
	return Health.CurrentHealth / Health.MaximumHealth;
}
