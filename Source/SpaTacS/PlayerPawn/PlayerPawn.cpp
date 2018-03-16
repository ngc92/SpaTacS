// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "PlayerPawnMovement.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

APlayerPawn::APlayerPawn()
{
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Spectator"));

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->bShouldUpdatePhysicsVolume = false;
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPlayerPawnMovement>(TEXT("Movement"));
	MovementComponent->UpdatedComponent = CollisionComponent;

	TargetMarker = CreateDefaultSubobject<USceneComponent>(TEXT("TargetMarker"));
	TargetMarker->AttachTo(RootComponent);

	bCanBeDamaged = false;
}

UPawnMovementComponent* APlayerPawn::GetMovementComponent() const
{
	return MovementComponent;
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CoupledActor)
	{
		auto Movement = Cast<UPlayerPawnMovement>(GetMovementComponent());
		Movement->ReferenceVelocity = CoupledActor->GetVelocity();
	}
}


void APlayerPawn::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
		}
	}
}

void APlayerPawn::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
		}
	}
}

void APlayerPawn::MoveUp(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Z), Val);

			UE_LOG(LogTemp, Warning, TEXT("VIEW Z %f"), GetActorLocation().Z);
		}
	}
}

