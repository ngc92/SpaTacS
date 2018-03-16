// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DistributionSystemComponent.generated.h"


class UMagazineComponent;
class UFuelTank;
class UPowerPlant;


USTRUCT(BlueprintType)
struct SPATACS_API FFuelTankStatus
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Capacity;
};


USTRUCT(BlueprintType)
struct SPATACS_API FPowerSystemStatus
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentConsumption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumGeneration;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UDistributionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDistributionSystemComponent();

    // Fuel

	/// Offers `Amount` kg of fuel into the system. Returns the amount of fuel actually taken.
	UFUNCTION(BlueprintCallable)
	float OfferFuel(float Amount);

	/// Requests `Amount` kg of fuel. Returns how much can be given.
	UFUNCTION(BlueprintCallable)
	float RequestFuel(float Amount);

    // Power
    /// Offers `Amount` Nm of energy to be used. Returns the amount of power actually wanted.
	UFUNCTION(BlueprintCallable)
    float OfferPower(float Amount);

    /// Requests `Amount` of Nm energy. Returns the actual amount of energy provided.
	UFUNCTION(BlueprintCallable)
    float RequestPower(float Amount);

    // Items
	UFUNCTION(BlueprintCallable)
    int RequestItem(const UClass* DesiredType, int Amount);

	UFUNCTION(BlueprintPure)
	FFuelTankStatus GetFuelInfo() const;

	UFUNCTION(BlueprintPure)
	FPowerSystemStatus GetPowerInfo() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// cached fuel
	float FuelCache;

	float StepFuelInflux;
	float StepFuelOutflux;

	float AverageFuelInflux;
	float AverageFuelOutflux;

	float FuelFullfillmentRatio;

    // cached power
    float GatheredEnergy;
    float CachedEnergy;

    float StepEnergyInflux;
    float StepEnergyOutflux;

    float AverageEnergyInflux;
    float AverageEnergyOutflux;

    float EnergyFullfillmentRatio;

    // Lists with relevant components
    UPROPERTY()
    TArray<UMagazineComponent*> Magazines;

	UPROPERTY()
	TArray<UFuelTank*> FuelTanks;

	UPROPERTY()
	TArray<UPowerPlant*> PowerPlants;
};
