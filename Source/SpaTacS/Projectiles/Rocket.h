// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicProjectile.h"
#include "Rocket.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRocketEngineBurnedOut);


UCLASS()
class SPATACS_API ARocket : public ABasicProjectile
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARocket();

    // Called every frame
    void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetTarget(FVector Position, FVector Velocity);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

    /// Burn time of the rocket.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BurnTime;

    /// Acceleration while burning
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Acceleration;

    // Dynamic Aiming Data
    FVector TargetPosition;
    FVector TargetVelocity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float BurnProgess;

#if WITH_EDITOR  
    void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif

    bool OnHitShip(AStarShip* OtherActor, UPrimitiveComponent* OtherComp, FVector IncomingVelocity, FVector DeltaV, const FHitResult& Hit) override;

    FVector CalculateSteeringVector(FVector Position, FVector Velocity) const;

    UPROPERTY(BlueprintAssignable)
    FOnRocketEngineBurnedOut OnRockedEngineBurnedOut;
};
