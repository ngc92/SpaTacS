// Fill out your copyright notice in the Description page of Project Settings.

#include "W_ArmourPiercing.h"
#include "StarShip.h"
#include "ShipSystems/ShipStructureComponent.h"

bool UW_ArmourPiercing::OnHitShip(AStarShip* Target, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit)
{
    // in MJ again.
    float Energy = DeltaV.SizeSquared() / 2 * GetMass() / 1000000;

    FVector IncomingDirection = IncomingVelocity.GetSafeNormal();
    if (Target->GetStructure()->DamagePiercingImpact(Hit.ImpactPoint, Energy, Caliber, -FVector::DotProduct(IncomingDirection, Hit.ImpactNormal)))
    {
        // Bounced, do not destroy yet.
        return true;
    }

	return false;
}


float UW_ArmourPiercing::GetMass() const
{
    float RadiusInCM = Caliber / 20;
    return (3.1415f * RadiusInCM * RadiusInCM) * (Length / 10.f) * Density / 1000.f;
}
