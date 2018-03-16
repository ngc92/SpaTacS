// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Warhead.h"
#include "W_HighExplosive.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Warhead), meta=(BlueprintSpawnableComponent))
class SPATACS_API UW_HighExplosive : public UWarhead
{
	GENERATED_BODY()
	
	
public:
	bool OnHitShip(AStarShip* Target, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit) override;

	float GetMass() const override;

protected:
	/// Explosive charge [MJ] of this warhead.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"))
	float Charge;
};
