// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipComponentHealth.h"

float UShipComponentHealth::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UShipComponentHealth::GetMaximumHealth() const
{
	return MaximumHealth;
}