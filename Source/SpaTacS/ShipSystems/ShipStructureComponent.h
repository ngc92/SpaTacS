// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShipStructureComponent.generated.h"


USTRUCT(BlueprintType)
struct SPATACS_API FArmourPlate
{
    GENERATED_BODY();
    
public:
    /// Current health points of this plate.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentHealth;

    /// Maximum health points of this plate.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaximumHealth;

    /// Relative position (w.r.t. the RootComponent) of the armour plate. For each impact the nearest plate will be respondible for deflecting the damage. 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RelativePosition;

    // Armour Data
    /// Armour thickness [cm] - reduces penetration chance.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Thickness;

    /// Armour hardness - reduces penetration chance and damage taken.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Hardness;

    // helpers
    /// penetraction [cm] for the given impact parameters.
    float CalculatePenetrationDepth(float ImpactEnergy, float Caliber, float AngleOfIncidence) const;

    void Damage(float Amount);
};


/// \todo this class should later manage attaching different components etc for dynamic ship construction.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPATACS_API UShipStructureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipStructureComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure)
    float GetHealthStatus() const;

    UFUNCTION(BlueprintPure)
    float GetMass() const;

    /// Calculate damage from a piercing impact. ImpactPoint is in world coordinates.
    bool DamagePiercingImpact(FVector ImpactPoint, float ImpactEnergy, float Caliber, float CosAngleOfIncidence);

    /// Calculate damage from a blunt source (crash/explosion).
	void DamageBluntImpact(FVector Center, float ImpactEnergy);

protected:
	/// Gets the armour plate that protects the ship at the given position.
	FArmourPlate* GetClosestPlate(FVector Position);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float MaximumStructuralHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float CurrentStructuralHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float StructureMass;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FArmourPlate> ArmourPlates;
};
