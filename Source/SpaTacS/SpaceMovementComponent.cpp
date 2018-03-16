// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include <cmath>

USpaceMovementComponent::USpaceMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bUpdateOnlyIfRendered = false;

	Velocity = { 0.f, 0.f, 0.f };
    LastVelocity = { 0.f, 0.f, 0.f };
    Acceleration = { 0.f, 0.f, 0.f };
    ForceBuffer = { 0.f, 0.f, 0.f };
    TorqueBuffer = { 0.f, 0.f, 0.f };
}

void USpaceMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// skip if don't want component updated when not rendered or updated component can't move
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(UpdatedComponent))
	{
		return;
	}

	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner)
	{
		return;
	}

    UpdatedPrimitive->AddForce(ForceBuffer);
    UpdatedPrimitive->AddTorqueInRadians(TorqueBuffer);
    Velocity = UpdatedPrimitive->GetPhysicsLinearVelocity();

    Acceleration = FMath::Lerp(Acceleration, (Velocity - LastVelocity) / DeltaTime, 1.f - std::exp(-10*DeltaTime));
    LastVelocity = Velocity;
    ForceBuffer = { 0.f, 0.f, 0.f };
    AngularVelocity = UpdatedPrimitive->GetPhysicsAngularVelocityInDegrees();
    TorqueBuffer = { 0.f, 0.f, 0.f };
}

void USpaceMovementComponent::AddThrust(FVector Thrust)
{
    ForceBuffer += Thrust;
}

void USpaceMovementComponent::SetVelocity(FVector NewVelocity)
{
    UpdatedPrimitive->SetPhysicsLinearVelocity(NewVelocity);
}

void USpaceMovementComponent::AddTorque(FVector Torque)
{
	// convert from rad to deg
    TorqueBuffer += Torque;
}

FVector USpaceMovementComponent::GetAcceleration() const
{
    return Acceleration;
}

FVector USpaceMovementComponent::GetAngularVelocity() const
{
    return AngularVelocity;
}

void USpaceMovementComponent::AddImpulseAtLocation(FVector Impulse, FVector WorldLocation)
{
    UpdatedPrimitive->AddImpulseAtLocation(Impulse, WorldLocation);
}

void USpaceMovementComponent::AddImpulseAtCoM(FVector Impulse)
{
    UpdatedPrimitive->AddImpulse(Impulse);
}

float USpaceMovementComponent::GetMass() const
{
	return UpdatedPrimitive->GetMass();
}
