// Fill out your copyright notice in the Description page of Project Settings.

#include "StarShip.h"
#include "ShipSystems/PropulsionControlComponent.h"
#include "ShipSystems/DistributionSystemComponent.h"
#include "ShipSystems/GunneryControlComponent.h"
#include "ShipSystems/ShipStructureComponent.h"
#include "SpaceMovementComponent.h"
#include "Components/SphereComponent.h"
#include <UObject/UObjectGlobals.h>


// Sets default values
AStarShip::AStarShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Propulsion = CreateDefaultSubobject<UPropulsionControlComponent>(TEXT("Propulsion"));
	DistributionSystem = CreateDefaultSubobject<UDistributionSystemComponent>(TEXT("DistributionSystem"));
	MovementComponent = CreateDefaultSubobject<USpaceMovementComponent>(TEXT("MovementComponent"));
    Gunnery = CreateDefaultSubobject<UGunneryControlComponent>(TEXT("Gunnery"));

    StructureComponent = CreateDefaultSubobject<UShipStructureComponent>(TEXT("Structure"));

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComponent->SetCanEverAffectNavigation(false);
    CollisionComponent->CanCharacterStepUpOn = ECB_No;
    CollisionComponent->bShouldUpdatePhysicsVolume = false;
    CollisionComponent->BodyInstance.bUseCCD = true;
    CollisionComponent->SetEnableGravity(false);
    CollisionComponent->SetSimulatePhysics(true);
    CollisionComponent->SetLinearDamping(0.f);
    RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void AStarShip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStarShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStarShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

