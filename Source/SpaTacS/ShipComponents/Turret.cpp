// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "GameFramework/Actor.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "SpaceMovementComponent.h"
#include <algorithm>


float FAimLimits::GetRequiredTurnAngle() const
{
    return std::max({ TurnFromAxis, TurnToAxis, 0.f });
}

// Sets default values for this component's properties
UTurret::UTurret()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    MinAngle = 0.f;
    MaxAngle = 90.f;
}


// Called when the game starts
void UTurret::BeginPlay()
{
	Super::BeginPlay();	

    DistributionSystem = GetOwner()->FindComponentByClass<UDistributionSystemComponent>();
    OwnerMovementComponent = GetOwner()->FindComponentByClass<USpaceMovementComponent>();
}


// Called every frame
void UTurret::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTurret::CanAimAt(const FVector& TargetDirection) const
{
    float angle = FQuat::FindBetweenNormals(GetForwardVector(), TargetDirection).GetAngle() * 180 / 3.1415;
    return angle >= MinAngle && angle <= MaxAngle;
}

FAimLimits UTurret::GetAimAngleData(const FVector& TargetDirection)
{
    float angle = FQuat::FindBetweenNormals(GetForwardVector(), TargetDirection).GetAngle() * 180 / 3.1415;
    
    if (MinAngle <= angle)
    {
        if (angle <= MaxAngle)
        {
            // both directions are possible => both values negative
            return FAimLimits(MinAngle - angle, angle - MaxAngle);
        }
        else
        {
            // towards is necessary => positive
            return FAimLimits(angle - MaxAngle, 0.f);
        }
    }
    else
    {
        // away is necessary => positive
        return FAimLimits(0.f, MinAngle - angle);
    }
}

FVector UTurret::GetAimDirectionForPosition(const FVector& TargetPosition) const
{
    FVector Delta = TargetPosition - GetComponentLocation();
    return Delta.GetSafeNormal();
}

FVector UTurret::GetMovingTargetAimPosition(const FVector& TargetPosition, const FVector& TargetSpeed, float ProjectileSpeed) const
{
    FVector SpeedDifference = TargetSpeed - GetOwner()->GetRootComponent()->ComponentVelocity;
    FVector AimPosition = TargetPosition;

    for (int i = 0; i < 2; ++i) {
        FVector Distance = AimPosition - GetComponentLocation();

        float TTI = Distance.Size() / ProjectileSpeed;
        AimPosition = TargetPosition + TTI * SpeedDifference;
    }

    return AimPosition;
}

void UTurret::SetAim(const FVector& Position, const FVector& Velocity)
{
    AimTargetPosition = Position;
    AimTargetVelocity = Velocity;
}
