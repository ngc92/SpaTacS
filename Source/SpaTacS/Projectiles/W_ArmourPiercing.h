// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Warhead.h"
#include "W_ArmourPiercing.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Warhead), meta=(BlueprintSpawnableComponent))
class SPATACS_API UW_ArmourPiercing : public UWarhead
{
	GENERATED_BODY()
	
public:
    bool OnHitShip(AStarShip* Target, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit) override;
	
    float GetMass() const override;

protected:
    /// Length [mm] of the warhead.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"))
    float Length;

    /// Density [g/cm�] of the shell. Together with Caliber this determines the shell's mass.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "5", UIMin = "5", UIMax = "20"))
    float Density;
};
