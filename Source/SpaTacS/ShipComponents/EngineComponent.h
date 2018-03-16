// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EngineComponent.generated.h"


class UDistributionSystemComponent;
class UPropulsionControlComponent;
class USpaceMovementComponent;


UCLASS( ClassGroup=(Spaceship), abstract, meta=(BlueprintSpawnableComponent) )
class SPATACS_API UEngineComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEngineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    /// Set to `true` to enable afterburner. This causes increased thrust
    /// at decreased fuel efficiency. The exact effect depends on the actual
    /// engine type.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bAfterBurner;

    /// Gets the current thrust vector in world coordinates [Newtons].
	FVector GetThrustVector() const;
    /// Gets the world direction in which thrust is applied.
	FVector GetThrustDirection() const;
    /// Gets an upper bound for the thrust that can be produced by this engine.
    /// Depending on external factors this amount might not actually be achievable.
	virtual float getMaximumThrust() const {
		check(0);
		return 0.f;
	}

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

protected:
    /// A reference to
	UPROPERTY()
	UDistributionSystemComponent* DistributionSystem;

    UPROPERTY()
    UPropulsionControlComponent* PropulsionSystem;

    UPROPERTY()
    USpaceMovementComponent* MovementComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float ProducedThrust;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
    float ThrustLevel;

    // PID Control
    FVector IntegralError;
    FVector LastError;

    UPROPERTY(EditAnywhere, Category=Control)
    float ProportionalControl;

    UPROPERTY(EditAnywhere, Category = Control)
    float DifferentialControl;

    UPROPERTY(EditAnywhere, Category = Control)
    float IntegralControl;

	UPROPERTY(EditAnywhere, Category = Control)
	float GlobalControlFactor;

private:
    virtual float ProduceThrust(float DeltaTime) { check(0); return 0.f; }
};
