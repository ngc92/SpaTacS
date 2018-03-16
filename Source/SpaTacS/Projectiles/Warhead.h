// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Warhead.generated.h"


class AStarShip;


UCLASS( abstract, ClassGroup=(Warhead), meta=(BlueprintSpawnableComponent) )
class SPATACS_API UWarhead : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarhead();

    virtual bool OnHitShip(AStarShip* Target, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit) {
        check(0);
		return false;
    };

    virtual float GetMass() const { return 1; }

    int GetCaliber() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    /// Caliber [mm] of the warhead, to determine which guns can fire which weapon.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"))
    int Caliber;
};
