// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicProjectile.h"
#include "Warhead.h"
#include "StarShip.h"
#include "SpaceMovementComponent.h"
#include "ShipSystems/ShipStructureComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include <Engine.h>
#include <cmath>



// Sets default values
ABasicProjectile::ABasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

    CollisionComponent->CanCharacterStepUpOn = ECB_No;
    CollisionComponent->bShouldUpdatePhysicsVolume = false;
    CollisionComponent->SetCanEverAffectNavigation(false);
    CollisionComponent->bDynamicObstacle = false;
    CollisionComponent->BodyInstance.bUseCCD = true;
    CollisionComponent->SetEnableGravity(false);
    CollisionComponent->SetSimulatePhysics(true);
    CollisionComponent->SetLinearDamping(0.f);

    CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetNotifyRigidBodyCollision(true);

    RootComponent = CollisionComponent;

    ProjectileMovement = CreateDefaultSubobject<USpaceMovementComponent>(TEXT("MovementComponent"));
    ProjectileMovement->UpdatedComponent = RootComponent;

    InitialLifeSpan = 60.f;
}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnHit);
	WarheadComponent = FindComponentByClass<UWarhead>();
}

void ABasicProjectile::SetVelocity(FVector TargetVelocity)
{
    ProjectileMovement->SetVelocity(TargetVelocity);
}

void ABasicProjectile::OnHit(UPrimitiveComponent* OwnCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor)
        return;

	bool destroy = true;
    // was it a space ship
    if (OtherActor->GetClass()->IsChildOf(AStarShip::StaticClass()))
    {
        // Estimate impact strength
        FVector DeltaV = NormalImpulse / OwnCom->GetMass();

        AStarShip* Enemy = Cast<AStarShip>(OtherActor);
		destroy = OnHitShip(Enemy, OtherComp, OwnCom->GetPhysicsLinearVelocity() - DeltaV, DeltaV, Hit);
    }

	if (destroy)
	{
		Destroy();
	}
}

bool ABasicProjectile::OnHitShip(AStarShip* Target, UPrimitiveComponent* OtherComp, FVector IncomingVelocity,
                                FVector DeltaV, const FHitResult& Hit)
{
    if (WarheadComponent)
    {
        return WarheadComponent->OnHitShip(Target, IncomingVelocity, DeltaV, Hit);
    }

	return false;
}

int ABasicProjectile::GetCaliber() const
{
    return WarheadComponent->GetCaliber();
}

#if WITH_EDITOR  
void ABasicProjectile::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	WarheadComponent = FindComponentByClass<UWarhead>();

    // Call the base class version  
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

