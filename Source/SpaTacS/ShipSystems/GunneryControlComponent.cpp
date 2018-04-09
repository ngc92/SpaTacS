// Fill out your copyright notice in the Description page of Project Settings.

#include "GunneryControlComponent.h"
#include "GameFramework/Actor.h"
#include "ShipComponents/Turret.h"


// Sets default values for this component's properties
UGunneryControlComponent::UGunneryControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunneryControlComponent::BeginPlay()
{
	Super::BeginPlay();

    TArray<UTurret*> Turrets;
    GetOwner()->GetComponents(Turrets);
    for (auto turret : Turrets)
    {
        TurretList.Add(FTurretData(turret));
    }
}


// Called every frame
void UGunneryControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bHoldFire)
        return;

    UpdateTargetSelectionProgress += DeltaTime;
    if (UpdateTargetSelectionProgress > UpdateTargetSelectionInterval)
    {
        UpdateTargetSelectionProgress = 0.f;
        for (auto& turret : TurretList)
        {
            SearchTargetForTurret(turret);
        }
    }

    for (auto& turret : TurretList)
    {
        if (turret.TargetActor) {
            if (turret.Turret->IsReadyToFire()) {
                turret.Turret->SetAim(turret.TargetActor->GetActorLocation(), turret.TargetActor->GetVelocity());
                turret.Turret->Fire();
            }
        }
    }
}

void UGunneryControlComponent::SearchTargetForTurret(FTurretData& TurretData)
{
    float best_score = 0.f;
    for (auto& Target : TargetList)
    {
		// check that target has a valid actor
		if(!Target.TargetActor) continue;
		
        FVector AimDir = TurretData.Turret->GetAimDirection(Target.TargetActor->GetActorLocation(), Target.TargetActor->GetVelocity());
        // For each possible target, get the rotation angle we need to fire.

        float aimErr = TurretData.Turret->GetAimAngleData(AimDir).GetRequiredTurnAngle();

        /// \todo consider potential for damage. Make target selection somewhat distance dependent etc.
        float score = Target.Priority / (aimErr + 0.1f);
        if (score > best_score)
        {
            TurretData.TargetActor = Target.TargetActor;
            TurretData.TargetPriority = Target.Priority;
            best_score = score;
        }
    }
}

struct TargetInformation
{
    TargetInformation(const FVector& AimDir, float priority) : 
        DesiredAimDirection(AimDir), Priority(priority)
    {

    }
    FVector DesiredAimDirection;
    float Priority;
};

/*
FVector UGunneryControlComponent::GetOptimalAttackHeading() const
{
    TArray<TargetInformation, TInlineAllocator<16>> AimInfo;
    FVector Direction;
    for (auto Turret : TurretList)
    {
        auto Target = Turret.TargetActor;
        if (!Target)
            continue;

        FVector AimDir = Turret.Turret->GetAimDirection(Target->GetActorLocation(), Target->GetVelocity());
        FAimLimits AimLimits = Turret.Turret->GetAimAngleData(AimDir);
        AimInfo.Add(TargetInformation(AimDir, Turret.TargetPriority));
    }
}
*/