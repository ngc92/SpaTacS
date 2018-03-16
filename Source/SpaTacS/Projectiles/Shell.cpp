// Fill out your copyright notice in the Description page of Project Settings.

#include "Shell.h"
#include "StarShip.h"
#include "SpaceMovementComponent.h"
#include "ShipSystems/ShipStructureComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "W_ArmourPiercing.h"
#include <Engine.h>
#include <cmath>

// Sets default values
AShell::AShell()
{
    PrimaryActorTick.bCanEverTick = false;
    InitialLifeSpan = 60.f;
    KineticEnergy = 10;  // About Leo 2
    CollisionComponent->InitSphereRadius(1.f);

    UpdateInitialSpeed();
}

void AShell::UpdateInitialSpeed()
{
	if(WarheadComponent) 
	{
		float Mass = WarheadComponent->GetMass();
		InitialSpeed = std::sqrt(2.f * KineticEnergy / Mass * 1000000);

		UE_LOG(LogTemp, Warning, TEXT("INITIAL SPEED %f"), InitialSpeed);

		CollisionComponent->SetMassOverrideInKg(NAME_None, Mass, true);
	}
}

void AShell::BeginPlay()
{
	Super::BeginPlay();
	UpdateInitialSpeed();
}

#if WITH_EDITOR  
void AShell::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateInitialSpeed();
}
#endif