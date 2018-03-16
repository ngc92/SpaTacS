// Fill out your copyright notice in the Description page of Project Settings.

#include "Rocket.h"
#include "StarShip.h"
#include "Warhead.h"
#include "SpaceMovementComponent.h"
#include "ShipSystems/ShipStructureComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include <Engine.h>
#include <cmath>

// Sets default values
ARocket::ARocket()
{
    PrimaryActorTick.bCanEverTick = true;
    InitialLifeSpan = 60.f;

    BurnProgess = 0.f;


    CollisionComponent->InitSphereRadius(1.f);
}

bool ARocket::OnHitShip(AStarShip* OtherActor, UPrimitiveComponent* OtherComp, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit)
{
	return Super::OnHitShip(OtherActor, OtherComp, IncomingVelocity, DeltaV, Hit);
}

void ARocket::SetTarget(FVector Position, FVector Velocity)
{
    TargetPosition = Position;
    TargetVelocity = Velocity;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
}

void ARocket::Tick(float DeltaTime)
{
    if (BurnProgess < BurnTime)
    {
		float Mass = ProjectileMovement->GetMass();
        // Some Aiming
        FVector TargetRelativeVelocity = TargetVelocity - ProjectileMovement->Velocity;
        FVector TargetRelativePosition = TargetPosition - GetActorLocation();

        FVector Steering = CalculateSteeringVector(TargetRelativePosition, TargetRelativeVelocity);
        float SteerAmount = Steering.Size();
        float StepDelta = Acceleration * DeltaTime;
        if (SteerAmount > StepDelta) {
            ProjectileMovement->AddImpulseAtCoM(Steering / SteerAmount * StepDelta * Mass);
        }
        else
        {
            ProjectileMovement->AddImpulseAtCoM(Steering * Mass);
            if (BurnProgess < BurnTime - 0.2)
            {
                FVector Acceleration = TargetRelativePosition.GetSafeNormal();
                ProjectileMovement->AddImpulseAtCoM(Acceleration * (StepDelta - SteerAmount) * Mass);
            }
        }

        BurnProgess += DeltaTime;
        TargetPosition += DeltaTime * TargetVelocity;
        if (BurnProgess > BurnTime)
        {
            OnRockedEngineBurnedOut.Broadcast();
        }
    }

     CollisionComponent->SetWorldRotation(ProjectileMovement->Velocity.Rotation());
}

FVector ARocket::CalculateSteeringVector(FVector Position, FVector Velocity) const
{
    float VelSize = Velocity.SizeSquared();

    // if we move at about the same velocity, just accelerate head on!
    if (VelSize < 5 * 5)
    {
        return Position;
    }

    float TimeTillClosestApproach = -FVector::DotProduct(Position, Velocity) / VelSize;

    // in the early acceleration phase the enemy might still be moving away from us. In that case
    // we just try to match our enemy's velocity.
    if (TimeTillClosestApproach < 0.f)
    {
        return Velocity;
    }
    else 
    {
        
        FVector ClosestApproach = Position + TimeTillClosestApproach * Velocity;
        float CADistance = ClosestApproach.SizeSquared();

        FVector ErrorCorrection = ClosestApproach / TimeTillClosestApproach;
        return ErrorCorrection;
    }

}

#if WITH_EDITOR  
void ARocket::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    // Call the base class version  
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif