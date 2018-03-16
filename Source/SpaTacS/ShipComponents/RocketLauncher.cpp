// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "SpaceMovementComponent.h"

void URocketLauncher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ReloadProgress += DeltaTime;
}

bool URocketLauncher::IsReadyToFire() const
{
    return ReloadProgress > ReloadTime;
}

bool URocketLauncher::Fire()
{
    if (!IsReadyToFire() || !RocketType) {
        return false;
    }

    ARocket* DefaultRocketObject = Cast<ARocket>(RocketType->GetDefaultObject());
    if (!DefaultRocketObject) {
        return false;
    }

    FVector TargetDirection = GetAimDirection(AimTargetPosition, AimTargetVelocity);
    if (!CanAimAt(TargetDirection))
        return false;

    AddRelativeRotation(FQuat::FindBetweenNormals(GetForwardVector(), TargetDirection));

    /// phi = tan(phi) = Precision / 1 km 
    FVector TargetVelocity = TargetDirection * LaunchSpeed;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    ARocket* spawned = GetOwner()->GetWorld()->SpawnActor<ARocket>(RocketType, GetComponentLocation(), GetComponentRotation());
    check(spawned);
    spawned->SetVelocity(TargetVelocity + OwnerMovementComponent->Velocity);
    spawned->SetTarget(AimTargetPosition, AimTargetVelocity);

    /// \todo enable this again when the ships have learned to maneuver.
    //FVector Impulse = TargetVelocity * spawned->GetMass();
    //OwnerMovementComponent->AddImpulseAtLocation(-Impulse, GetComponentLocation());

    ReloadProgress = 0;
    if (DistributionSystem->RequestItem(RocketType->GetClass(), 1) != 1)
    {
        RocketType = nullptr;
    }
    FiredEvent.Broadcast();
    return true;
}

FVector URocketLauncher::GetAimDirection(const FVector& TargetPosition, const FVector& TargetVelocity) const
{
    return GetAimDirectionForPosition(GetMovingTargetAimPosition(AimTargetPosition, AimTargetVelocity, LaunchSpeed));
}