// Fill out your copyright notice in the Description page of Project Settings.

#include "W_HighExplosive.h"
#include "StarShip.h"
#include "ShipSystems/ShipStructureComponent.h"




bool UW_HighExplosive::OnHitShip(AStarShip* Target, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HE"));
	// in MJ again.
	float Energy = DeltaV.SizeSquared() / 2 * GetMass() / 1000000;

	FVector IncomingDirection = IncomingVelocity.GetSafeNormal();
	if (Target->GetStructure()->DamagePiercingImpact(Hit.ImpactPoint, 0.2*Energy, Caliber, -FVector::DotProduct(IncomingDirection, Hit.ImpactNormal)))
	{
	}

	Target->GetStructure()->DamageBluntImpact(Hit.ImpactPoint, Charge);

	return false;
}

float UW_HighExplosive::GetMass() const
{
	float RadiusInCM = Caliber / 20;
	return (3.1415f * RadiusInCM * RadiusInCM) * (2* RadiusInCM / 10.f) * 3.f / 1000.f + 0.1f * Charge;
}