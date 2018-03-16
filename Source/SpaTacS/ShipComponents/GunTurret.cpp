// Fill out your copyright notice in the Description page of Project Settings.

#include "GunTurret.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Projectiles/Shell.h"
#include "SpaTacSGameModeBase.h"
#include "SpaceMovementComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"

void UGunTurret::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (RoundProgress * RPM < 60) {
        RoundProgress += DeltaTime;
    }

    if (CurrentAmmo == 0)
    {
        CurrentAmmo += DistributionSystem->RequestItem(ShellType->GetClass(), MagazineCapacity);
    }
}

bool UGunTurret::IsReadyToFire() const
{
    return RoundProgress * RPM >= 60 && CurrentAmmo > 0;
}

bool UGunTurret::Fire()
{
    if (RoundProgress * RPM < 60  || CurrentAmmo <= 0 || !ShellType) {
        return false;
    }

    AShell* DefaultShellObject = Cast<AShell>(ShellType->GetDefaultObject());
    if (!DefaultShellObject) {
        return false;
    }

    FVector TargetDirection = GetAimDirection(AimTargetPosition, AimTargetVelocity);
    if (!CanAimAt(TargetDirection))
        return false;

    AddRelativeRotation(FQuat::FindBetweenNormals(GetForwardVector(), TargetDirection));
    
    /// phi = tan(phi) = Precision / 1 km 
    FVector TargetVelocity = FMath::VRandCone(TargetDirection, Precision / 1000) * DefaultShellObject->GetInitialSpeed();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    AShell* spawned = GetOwner()->GetWorld()->SpawnActor<AShell>(ShellType, GetComponentLocation(), FRotator(0.f, 0.f, 0.f));
    check(spawned);
    spawned->SetVelocity(TargetVelocity + OwnerMovementComponent->Velocity);

    /// \todo enable this again when the ships have learned to maneuver.
    //FVector Impulse = TargetVelocity * spawned->GetMass();
    //OwnerMovementComponent->AddImpulseAtLocation(-Impulse, GetComponentLocation());

    RoundProgress -= 60 / RPM;
    CurrentAmmo -= 1;
    FiredEvent.Broadcast();
    return true;
}

FVector UGunTurret::GetAimDirection(const FVector& TargetPosition, const FVector& TargetVelocity) const
{
    AShell* DefaultShellObject = Cast<AShell>(ShellType->GetDefaultObject());
    if (!DefaultShellObject)
    {
        return FVector(0, 0, 0);
    }
    return GetAimDirectionForPosition(GetMovingTargetAimPosition(AimTargetPosition, AimTargetVelocity, DefaultShellObject->GetInitialSpeed()));
}