// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipStructureComponent.h"
#include "GameFramework/Actor.h"
#include "SpaTacS.h"
#include <cmath>


// Sets default values for this component's properties
UShipStructureComponent::UShipStructureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShipStructureComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UShipStructureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UShipStructureComponent::GetHealthStatus() const
{
    return CurrentStructuralHealth / MaximumStructuralHealth;
}

float UShipStructureComponent::GetMass() const
{
    return StructureMass;
}

bool UShipStructureComponent::DamagePiercingImpact(FVector ImpactPoint, float ImpactEnergy, float Caliber, float CosAngleOfIncidence)
{
    // figure out where we were hit
    FArmourPlate* HitPlate = GetClosestPlate(ImpactPoint);
    check(HitPlate);

    // check for penetration
    float PenetrationDepth = HitPlate->CalculatePenetrationDepth(ImpactEnergy, Caliber, CosAngleOfIncidence);
	float PlateHealthFactor = HitPlate->CurrentHealth / HitPlate->MaximumHealth;
    float ArmourThickness = HitPlate->Thickness;
    float ActualPenetration = FMath::RandRange(0.f, PenetrationDepth) + FMath::RandRange(0.f, PenetrationDepth);
	ActualPenetration *= 2 / (1 + PlateHealthFactor) * (FMath::FRand() + 0.5);

	float PenetractionFactor = ActualPenetration / ArmourThickness;

	UE_LOG(LogDamageEvents, Log, 
		TEXT("Ship '%s' takes %.1f MJ of piercing damage from caliber %.1f at cos(angle) %f. "
			 "Armour penetration %.1f %% at %.1f mm againt nominal armour strength %.1f mm."), 
		*GetOwner()->GetName(), ImpactEnergy, Caliber, CosAngleOfIncidence, 100* PenetractionFactor, ActualPenetration, HitPlate->Thickness);

    float ArmourDamage = ImpactEnergy;
    float InternalDamage = 0.f;
    bool bBounce = false;
    if (ActualPenetration > ArmourThickness)
    {
		UE_LOG(LogDamageEvents, Log, TEXT("Full Penetration"));
		float InternalFactor = (PenetractionFactor - 1) / PenetractionFactor;
        // Pierce Armour
        ArmourDamage = ImpactEnergy * (1 - InternalFactor);
        InternalDamage = ImpactEnergy * InternalFactor;
    }
    else if (ActualPenetration > ArmourThickness * FMath::FRand())
    {
		UE_LOG(LogDamageEvents, Log, TEXT("Partial Penetration"));
        // Damage Armour
        ArmourDamage *= 0.9;
        InternalDamage = ImpactEnergy * 0.1;
    }
    else
    {
		UE_LOG(LogDamageEvents, Log, TEXT("Bounce"));
        // Bounce
        ArmourDamage *= 0.1;
        bBounce = true;
    }

	// maximum armour damage given by caliber.
	float MaxArmourDamage = (Caliber * HitPlate->Thickness) / 1000.f;

	if (ArmourDamage > MaxArmourDamage)
	{
		float Factor = std::sqrt(ArmourDamage / MaxArmourDamage);
		float NewArmourDamage = MaxArmourDamage * Factor;
		InternalDamage += ArmourDamage - NewArmourDamage;
		ArmourDamage = NewArmourDamage;
	}

    // Apply Armour Damage
    HitPlate->Damage(ArmourDamage);

	// Cap internal damage
	float MaxInternalDamage = std::sqrt(MaximumStructuralHealth / Caliber) * 0.1;
	if (InternalDamage > MaxInternalDamage)
	{
		float Factor = std::sqrt(InternalDamage / MaxInternalDamage);
		InternalDamage = MaxInternalDamage * Factor;
	}

    // Apply internal damage
    CurrentStructuralHealth -= InternalDamage;

	UE_LOG(LogDamageEvents, Log, TEXT("Hit does %.1f damage against armour (%.1f HP) and %.1f damage against ship structure (%.1f HP)"), ArmourDamage, HitPlate->CurrentHealth, 
		InternalDamage, CurrentStructuralHealth);

    return bBounce;
}

void UShipStructureComponent::DamageBluntImpact(FVector Center, float ImpactEnergy)
{
	UE_LOG(LogDamageEvents, Warning, TEXT("Ship '%s' takes %f MJ of blunt damage."), *GetOwner()->GetFullName(), ImpactEnergy);
	float ArmourDamage = ImpactEnergy * 0.5;
	float InternalDamage = ImpactEnergy * 0.1;

	/// \todo make interal damage depend on armour status

	// figure out where we were hit
	FArmourPlate* HitPlate = GetClosestPlate(Center);
	check(HitPlate);

	// Apply Armour Damage
	HitPlate->Damage(ArmourDamage);
}

FArmourPlate* UShipStructureComponent::GetClosestPlate(FVector Position)
{
	/// \todo check that this is correct.
	Position = GetOwner()->GetActorTransform().InverseTransformPosition(Position);

	// figure out where we were hit
	float MinDistSq = 1e10f;
	FArmourPlate* HitPlate = nullptr;
	for (auto& Plate : ArmourPlates)
	{
		float DistSq = Plate.RelativePosition.SizeSquared();
		if (DistSq < MinDistSq)
		{
			HitPlate = &Plate;
			MinDistSq = DistSq;
		}
	}
	
	check(HitPlate);
	return HitPlate;
}





float FArmourPlate::CalculatePenetrationDepth(float ImpactEnergy, float Caliber, float CosAngleOfIncidence) const
{
    // inspired by https://de.wikipedia.org/wiki/Panzerformel
    float E5 = std::pow(ImpactEnergy, 5.f);
    float k = std::pow(Caliber, 7.5f);

    float angle_factor = std::pow(CosAngleOfIncidence, 1.5f);

    return 1e3f * angle_factor * std::pow(E5 / k, 1.f / 7.f) / Hardness;
}

void FArmourPlate::Damage(float Amount)
{
    if (Amount < CurrentHealth)
    {
        CurrentHealth -= Amount;
    }
    else
    {
        CurrentHealth = 0.f;
    }
}