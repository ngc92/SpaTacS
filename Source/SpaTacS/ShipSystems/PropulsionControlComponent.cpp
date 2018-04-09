// Fill out your copyright notice in the Description page of Project Settings.

#include "PropulsionControlComponent.h"
#include "SpaceMovementComponent.h"
#include "GameFramework/Actor.h"
#include "ShipComponents/Propulsion/EngineComponent.h"
#include "ShipComponents/Propulsion/ThrusterComponent.h"
#include "DistributionSystemComponent.h"
#include <cmath>
#include <Kismet/KismetMathLibrary.h>


// Sets default values for this component's properties
UPropulsionControlComponent::UPropulsionControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    bControlVelocity = true;
    bControlRotation = true;
	DesiredVelocity = { 0.f, 0.f, 0.f };
	MaximumAcceleration = 9.81 * 2;
    MaximumAngularVelocity = 30;
}


// Called when the game starts
void UPropulsionControlComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents(Engines);
	GetOwner()->GetComponents(Thrusters);
	MovementComponent = GetOwner()->FindComponentByClass<USpaceMovementComponent>();
}

// Called every frame
void UPropulsionControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MovementComponent) {
		return;
	}

	if (bControlVelocity)
	{
        FVector Error = DesiredVelocity - MovementComponent->Velocity;
		FVector TargetAcc = ProportionalVelocityControl * Error + DifferentialVelocityControl * (Error - LastVelocityError) / DeltaTime;
        LastVelocityError = Error;

        float TargetAccelerationSizeSquared = TargetAcc.SizeSquared();

        if (TargetAccelerationSizeSquared > MaximumAcceleration * MaximumAcceleration)
        {
            float len = TargetAcc.Size();
            TargetAcc *= MaximumAcceleration / len;
        }
        else if (TargetAccelerationSizeSquared < VelocityTolerance * VelocityTolerance)
        {
            TargetAcc *= 0.f;
        }

        DesiredAcceleration = TargetAcc;
	}

    if (bRotateToAlignThrust)
    {
        FVector IdealThrustVector = { 0.f, 0.f, 0.f };
        for (UEngineComponent* Engine : Engines)
        {
            IdealThrustVector += Engine->getMaximumThrust() * Engine->GetThrustDirection();
        }

        SmoothedTargetAcceleration = FMath::Lerp(DesiredAcceleration, SmoothedTargetAcceleration , std::exp(-DeltaTime));
        float STA_Size = SmoothedTargetAcceleration.Size();

        FQuat QError = FQuat::FindBetweenVectors(IdealThrustVector, SmoothedTargetAcceleration);
        FRotator TargetRotation = FRotator(QError * GetOwner()->GetActorQuat());


        DesiredRotation = FMath::Lerp(TargetRotation, DesiredRotation, std::exp(-DeltaTime));
        DesiredRotation.Roll = GetOwner()->GetActorRotation().Roll;
    }

    if (bControlPosition)
    {
		FVector BaseLocation = GetOwner()->GetActorLocation();
		// planning with brake maneuver. We need still need to do braking when velocity and distance are aligned.
		/*if (FVector::DotProduct(LastPositionError, MovementComponent->Velocity) > 1000)
		{
			float RotateTime = 180 / MaximumAngularVelocity;
			BaseLocation += MovementComponent->Velocity * RotateTime;
		}*/
		FVector Error = DesiredPosition - BaseLocation;
		FVector DError = Error - LastPositionError;
        FVector TargetVel = ProportionalPositionControl * Error + (DifferentialPositionControl / DeltaTime) * DError;
        LastPositionError = Error;

        float TargetVelocitySizeSquared = TargetVel.SizeSquared();

        if (TargetVelocitySizeSquared > MaximumSpeed * MaximumSpeed)
        {
            float len = TargetVel.Size();
            TargetVel *= MaximumSpeed / len;
        }

        DesiredVelocity = TargetVel;
    }

    if (bControlRotation)
    {
        FVector FV1 = FQuat(DesiredRotation).GetForwardVector();
        FVector FV2 = GetOwner()->GetActorQuat().GetForwardVector();
        FVector RotationAxis;
        float RotationAngle;
        FQuat::FindBetweenNormals(FV2, FV1).ToAxisAndAngle(RotationAxis, RotationAngle);
        FVector PError = RotationAxis * RotationAngle * 180 / 3.1415;

        FVector DifferentialError = PError - LastRotationError;
        LastRotationError = PError;
        DesiredAngularVelocity = (PError * ProportionalAngleControl * DeltaTime + DifferentialAngleControl * DifferentialError) * MaximumAngularVelocity;
        
        float angvel = DesiredAngularVelocity.Size();
        if (angvel > MaximumAngularVelocity)
        {
            DesiredAngularVelocity *= MaximumAngularVelocity / angvel;
        } 
    }
}

FVector UPropulsionControlComponent::GetCurrentAcceleration() const
{
    return MovementComponent->GetAcceleration();
}

float UPropulsionControlComponent::GetShipMass() const
{
	return MovementComponent->GetMass();
}
