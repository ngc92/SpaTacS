// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrusterComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "ShipSystems/PropulsionControlComponent.h"
#include "SpaceMovementComponent.h"
#include "GameFramework/Actor.h"
#include <cmath>


// Sets default values for this component's properties
UThrusterComponent::UThrusterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    DesiredDriftThrust = FVector(0.f, 0.f, 0.f);
    LastAccelerationError = { 0.f, 0.f, 0.f };
}


// Called when the game starts
void UThrusterComponent::BeginPlay()
{
	Super::BeginPlay();

    DistributionSystem = GetOwner()->FindComponentByClass<UDistributionSystemComponent>();
    check(DistributionSystem);

    PropulsionSystem = GetOwner()->FindComponentByClass<UPropulsionControlComponent>();
    check(DistributionSystem);

    MovementComponent = GetOwner()->FindComponentByClass<USpaceMovementComponent>();
    check(MovementComponent);
}


// Called every frame
void UThrusterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Drift Control
    float MaxDrift = GetMaximumDriftAcceleration();
    if(MaxDrift > 0)  
    {
        FVector Error = PropulsionSystem->DesiredAcceleration - PropulsionSystem->GetCurrentAcceleration();
        FVector DifferentialError = Error - LastAccelerationError;
        LastAccelerationError = Error;
        DesiredDriftThrust *= std::exp(-DriftControlDecay * DeltaTime);  // Let it decay to encourage use of main engine! 
        DesiredDriftThrust += DeltaTime * ProportionalDriftControl * Error + DifferentialDriftControl * DifferentialError;

        // Cap desired drift at maximum attainable drift.
        if(DesiredDriftThrust.SizeSquared() > MaxDrift * MaxDrift)
        {
            DesiredDriftThrust *= MaxDrift / DesiredDriftThrust.Size();
        }
    }
    else
    {
        DesiredDriftThrust = { 0.f, 0.f, 0.f };
    }

    float MaxTorque = GetMaximumTorque();
    if (MaxTorque > 0)
    {
        // Angular contror
        FVector MissingRotation = PropulsionSystem->DesiredAngularVelocity - MovementComponent->GetAngularVelocity();
        FVector DesiredTorque = DeltaTime * ProportionalSpinControl * MissingRotation + DifferentialSpinControl * (MissingRotation - LastAngularMomentumError);
        LastAngularMomentumError = MissingRotation;
        DesiredTorque *= GetMaximumTorque();
        float desiredRotationSq = DesiredTorque.SizeSquared();

        if(desiredRotationSq > MaxTorque * MaxTorque)
        {
            DesiredTorque *= MaxTorque / std::sqrt(desiredRotationSq);
        }
        ProducedTorque = ProduceTorque(DeltaTime, DesiredTorque);
    }
    else
    {
        ProducedTorque = { 0.f, 0.f, 0.f };
    }

    ProducedDrift = ProduceDrift(DeltaTime, DesiredDriftThrust);
   
    // Doing the update here means that we have one frame of delay between the engine producing thrust and it 
    // being applied to the ship. That is okay.
	if(MovementComponent)
    {
        MovementComponent->AddThrust( ProducedDrift );
        MovementComponent->AddTorque( ProducedTorque );
    }
}

FVector UThrusterComponent::GetProducedDrift() const
{
	return ProducedDrift;
}

FVector UThrusterComponent::GetProducedTorque() const
{
	return ProducedTorque;
}
