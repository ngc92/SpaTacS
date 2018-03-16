// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunneryControlComponent.generated.h"

class UTurret;


USTRUCT(BlueprintType)
struct SPATACS_API FTargetData
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    const AActor* TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;
};


USTRUCT(BlueprintType)
struct SPATACS_API FTurretData
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTurret* Turret = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    const AActor* TargetActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetPriority;

public:
    FTurretData() = default;

    FTurretData(UTurret* turret) : Turret(turret)
    {
    }
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UGunneryControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunneryControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
    TArray<FTargetData> TargetList;

    UPROPERTY()
    TArray<FTurretData> TurretList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHoldFire = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UpdateTargetSelectionInterval;

private:
    float UpdateTargetSelectionProgress = 0.f;

    /// Go through the list of possible targets and find the one that turret should fire at. Sets this as Target in `TurretData`.
    void SearchTargetForTurret(FTurretData& TurretData);

    /// Check where the Turrets want to fire to, and calculate the optimal steering vector
    //FVector GetOptimalAttackHeading() const;
};
