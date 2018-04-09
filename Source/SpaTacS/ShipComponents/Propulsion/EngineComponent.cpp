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
	if(!MovementComponent || !PropulsionSystem || DeltaTime < 1e-6)
		return;

    // Control
    FVector Error = PropulsionSystem->DesiredAcceleration - PropulsionSystem->GetCurrentAcceleration();
	
    float directionalOverlap = FVector::DotProduct(GetThrustDirection(), Error);
	// be somewhat optimistic about performance. also eliminates danger of div by zero.
	float max_acc = getMaximumThrust() / PropulsionSystem->GetShipMass() + 0.1f;
	float change = directionalOverlap / DeltaTime / max_acc;
	UpdateThrustLevel(change, DeltaTime);

    ProducedThrust = ProduceThrust(DeltaTime);
    MovementComponent->AddThrust( GetThrustVector() );
}

void UEngineComponent::UpdateThrustLevel(float Change, float DeltaTime)
{
	// limit the rate of change
	if(Change > 1.f) {
		Change = 1.f;
	}
	
	if(Change < -1.f) {
		Change = -1.f;
	}
	
	ThrustLevel += Change * DeltaTime;
	
	// limit the thrust level
	if (ThrustLevel < 0.f)
    {
		ThrustLevel = 0.f;
	}
	else if (ThrustLevel > 1.f)
    {
		ThrustLevel = 1.f;
	}
}
