// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponents/Turret.h"
#include "GunTurret.generated.h"


class AShell;

/**
 * A Gun Turrent is a Turret that fires non self-propelled bullets and shells.
 */
UCLASS(ClassGroup = (Spaceship), meta = (BlueprintSpawnableComponent))
class SPATACS_API UGunTurret : public UTurret
{
	GENERATED_BODY()
	
public:
    bool Fire() override;

    bool IsReadyToFire() const override;

    // Called every frame
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FVector GetAimDirection(const FVector& TargetPosition, const FVector& TargetVelocity) const override;

protected:
    /// Number of rounds per minute this turret can shoot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RPM;

    /// incremented with `DeltaTime` each step. Ready to fire if this exceeds `SecsPerShot`.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    float RoundProgress;

    /// Maximum capacity of the magazine
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MagazineCapacity;

    /// amount of shells currently in magazine
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentAmmo;

    /// Type of the currently loaded shell.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AShell> ShellType;

    /// Minimum caliber this gun can shoot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinCaliber;

    /// Maximum caliber this gun can shoot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxCaliber;

    /// Precision of the gun. Deviation (in m) at range of 1 km.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Precision;
};
