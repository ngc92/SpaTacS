// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponentInterface.generated.h"


USTRUCT(BlueprintType)
struct SPATACS_API FHealthData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float MaximumHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float CurrentHealth;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UShipComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPATACS_API IShipComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	float GetCurrentHealth() const;

	float GetMaximumHealth() const;

	float GetHealthStatus() const;

private:
	virtual FHealthData & GetHealthData() = 0;
	virtual const FHealthData & GetHealthData() const = 0;
};
