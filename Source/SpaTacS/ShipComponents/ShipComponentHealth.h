// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponentHealth.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Spaceship))
class SPATACS_API UShipComponentHealth : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaximumHealth() const;

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaximumHealth;
};
