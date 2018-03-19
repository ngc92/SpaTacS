// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponentBase.h"
#include "Turret.generated.h"


class UDistributionSystemComponent;
class USpaceMovementComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFiredEvent);


USTRUCT(BlueprintType)
struct SPATACS_API FAimLimits
{
    GENERATED_BODY();

public:
    FAimLimits() : TurnToAxis(0), TurnFromAxis(0) { }
    FAimLimits(float To, float From) : TurnToAxis(To), TurnFromAxis(From) { }

    /// Angle (in radians) of turn possible / required toward forward axis of Turret.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurnToAxis;

    /// Angle (in radians) of turn possible / required away from forward axis of Turret.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurnFromAxis;

    /// Gets the magnitude of the angle that is required to make aiming possible.
    float GetRequiredTurnAngle() const;
};


UCLASS(Blueprintable, ClassGroup = (Spaceship), abstract, meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UTurret : public UShipComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurret();

    FFiredEvent& OnFired() { return FiredEvent; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector AimTargetPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector AimTargetVelocity;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /// Returns whether `TargetDirection` lies within the firing cone of this turret.
    bool CanAimAt(const FVector& TargetDirection) const;
    /// Returns the amount of turning necessary (positive) or maximally possible (negative) to still aim at `TargetDiretion`. 
    FAimLimits GetAimAngleData(const FVector& TargetDirection);


    // Aiming Helpers. These are basic Math functions that need not be overridden by derived classes.
    FVector GetAimDirectionForPosition(const FVector& TargetPosition) const;
    FVector GetMovingTargetAimPosition(const FVector& TargetPosition, const FVector& TargetSpeed, float ProjectileSpeed) const;

    /// Gets the aim direction necessary to hit target with current ammunition.
    virtual FVector GetAimDirection(const FVector& TargetPosition, const FVector& TargetVelocity) const {
        check(0); return FVector();
    }

    void SetAim(const FVector& Position, const FVector& Velocity);

    virtual bool Fire() { check(0); return false; }
    virtual bool IsReadyToFire() const { check(0); return false; }

protected:
    UPROPERTY()
    UDistributionSystemComponent* DistributionSystem;

    UPROPERTY()
    USpaceMovementComponent* OwnerMovementComponent;

    UPROPERTY(BlueprintAssignable)
    FFiredEvent FiredEvent;
};
