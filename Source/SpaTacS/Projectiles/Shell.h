// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicProjectile.h"
#include "Shell.generated.h"

class USpaceMovementComponent;
class USphereComponent;


UCLASS()
class SPATACS_API AShell : public ABasicProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShell();

    // Getters
    float GetInitialSpeed() const { return InitialSpeed; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

    // Shell configuration
    /// Kinetic energy [MJ] this shell will have when shot. Reference: Leo 2: 10 MJ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin = "0"))
    float KineticEnergy;

    /// Initial speed of the shell. Derived from kinetic energy.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float InitialSpeed;

#if WITH_EDITOR  
    void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif

	void UpdateInitialSpeed();
};
