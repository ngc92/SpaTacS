// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "ShipSystems/PropulsionControlComponent.h"
#include "SpaceMovementComponent.h"
#include "GameFramework/Actor.h"
#include <cmath>


// Sets default values for this component's properties
UEngineComponent::UEngineComponent()
{
	ThrustLevel = 0.f;
    ProportionalControl = 1.f;
    DifferentialControl = 1.f;
    IntegralControl = 1.f;
	GlobalControlFactor = 0.01f;
    
	bAfterBurner = false;
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEngineComponent::BeginPlay()
{
	Super::BeginPlay();

    DistributionSystem = GetOwner()->FindComponentByClass<UDistributionSystemComponent>();
    check(DistributionSystem);

    PropulsionSystem = GetOwner()->FindComponentByClass<UPropulsionControlComponent>();
    check(DistributionSystem);

    MovementComponent = GetOwner()->FindComponentByClass<USpaceMovementComponent>();
    check(MovementComponent);
}

FVector UEngineComponent::GetThrustVector() const
{
	return -ProducedThrust * GetForwardVector();
}

FVector UEngineComponent::GetThrustDirection() const
{
	return -GetForwardVector();
}

// Called every frame
void UEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Control
    FVector Error = PropulsionSystem->DesiredAcceleration - PropulsionSystem->GetCurrentAcceleration();
    FVector Derivative = (Error - LastError) / DeltaTime;
    LastError = Error;
    IntegralError += DeltaTime * Error;
    IntegralError *= std::exp(-DeltaTime);

    FVector ControlVector = ProportionalControl * Error + IntegralControl * IntegralError + DifferentialControl * Derivative;
    float ControlStrength = ControlVector.Size();
    if(ControlStrength > 1e-6) {
        ControlVector *= 1.0 / ControlStrength;
    }

    float directionalOverlap = FVector::DotProduct(GetThrustDirection(), ControlVector);
    ThrustLevel += GlobalControlFactor * ControlStrength * (4*directionalOverlap - 3) * DeltaTime;

    if (ThrustLevel < 0.f)
    {
		ThrustLevel = 0.f;
	}
	else if (ThrustLevel > 1.f)
    {
		ThrustLevel = 1.f;
	}

    ProducedThrust = ProduceThrust(DeltaTime);
    if(MovementComponent)
    {
        MovementComponent->AddThrust( GetThrustVector() );
    }   
}
