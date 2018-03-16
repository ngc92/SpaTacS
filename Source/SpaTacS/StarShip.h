// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StarShip.generated.h"


class UPropulsionControlComponent;
class UDistributionSystemComponent;
class USpaceMovementComponent;
class UGunneryControlComponent;
class UShipStructureComponent;
class USphereComponent;


UCLASS()
class SPATACS_API AStarShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStarShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ShipName;

	UPropulsionControlComponent* GetPropulsion() { return Propulsion; }
    UShipStructureComponent* GetStructure() { return StructureComponent; }

protected:	
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category=Systems)
	UPropulsionControlComponent* Propulsion;

    UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Systems)
    UGunneryControlComponent* Gunnery;

	UPROPERTY(BlueprintReadOnly, NoClear, Category = Systems)
	UDistributionSystemComponent* DistributionSystem;

	UPROPERTY(VisibleAnywhere, NoClear, BlueprintReadOnly, Category = Movement)
	USpaceMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UShipStructureComponent* StructureComponent;
};
