// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Rocket.h"
#include "ShipComponents/Turret.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Spaceship), meta = (BlueprintSpawnableComponent))
class SPATACS_API URocketLauncher : public UTurret
{
	GENERATED_BODY()

public:
    bool Fire() override;

    bool IsReadyToFire() const override;

    // Called every frame
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FVector GetAimDirection(const FVector& TargetPosition, const FVector& TargetVelocity) const override;

protected:
    /// Time delay between two consecutive rockets fired.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadTime;

    /// incremented with `DeltaTime` each step. Ready to fire if this exceeds `ReloadTime`.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    float ReloadProgress;

    /// Type of the currently loaded rocket.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARocket> RocketType;

    /// Minimum caliber this launcher can shoot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinCaliber;

    /// Maximum caliber this launcher can shoot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxCaliber;

    /// Initial Speed with which rockets will be launched
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LaunchSpeed;
};
