// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicProjectile.generated.h"

class USpaceMovementComponent;
class USphereComponent;
class AStarShip;
class UWarhead;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileExplode);


UCLASS(abstract)
class SPATACS_API ABasicProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABasicProjectile();

    void SetVelocity(FVector TargetVelocity);

    /// Caliber information is used to determine which projectile can be shot from which weapon.
    virtual int GetCaliber() const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere)
    USpaceMovementComponent* ProjectileMovement;
	
    UPROPERTY(EditAnywhere)
    UWarhead* WarheadComponent;

    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* OwnCom, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit);

	#if WITH_EDITOR  
    void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
	#endif

	UPROPERTY(BlueprintAssignable)
	FOnProjectileExplode OnExplode;
					   
protected:
	/// return true if projectile should bounce.
    virtual bool OnHitShip(AStarShip* OtherActor, UPrimitiveComponent* OtherComp, FVector IncomingVelocity,
                           FVector DeltaV, const FHitResult& Hit);
};
