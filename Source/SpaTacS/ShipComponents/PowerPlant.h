// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponentBase.h"
#include "PowerPlant.generated.h"


class UDistributionSystemComponent;


UCLASS(ClassGroup = (Spaceship), abstract, meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UPowerPlant : public UShipComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPowerPlant();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	virtual float GetMaximumEnergyProduction() const {
		check(0);
		return 0.f;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY()
    UDistributionSystemComponent* DistributionSystem;

    float WorkLevel = 1.f;
private:
    virtual float ProduceEnergy(float DeltaTime) {
        check(0); 
        return 0.f;
    };
};
