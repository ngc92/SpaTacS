// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponents/ShipComponentBase.h"
#include "ThrusterComponent.generated.h"


class UDistributionSystemComponent;
class USpaceMovementComponent;
class UPropulsionControlComponent;


UCLASS( ClassGroup=(ShipComponent), abstract, meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UThrusterComponent : public UShipComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrusterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetProducedDrift() const;
	FVector GetProducedTorque() const;

    virtual float GetMaximumDriftAcceleration() const { check(0); return 0.f; }
    virtual float GetMaximumTorque() const { check(0); return 0.f; }

protected:
    // connection to other components
    UPROPERTY()
    UDistributionSystemComponent* DistributionSystem;

    UPROPERTY()
    USpaceMovementComponent* MovementComponent;

    UPROPERTY()
    UPropulsionControlComponent* PropulsionSystem;

    // control functions
    virtual FVector ProduceDrift(float DeltaTime, FVector Desired) { check(0); return { 0.f, 0.f, 0.f }; }
    virtual FVector ProduceTorque(float DeltaTime, FVector Desired) { check(0); return { 0.f, 0.f, 0.f }; }

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Control)
    FVector DesiredDriftThrust;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector ProducedDrift;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector ProducedTorque;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
    float ProportionalDriftControl = 0.1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
    float DifferentialDriftControl = 0.1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
    float ProportionalSpinControl = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
    float DifferentialSpinControl = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
    float DriftControlDecay = 0.1;

    FVector LastAccelerationError;
    FVector LastAngularMomentumError;
};
