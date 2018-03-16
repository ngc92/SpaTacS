// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuelTank.generated.h"


class UDistributionSystemComponent;


UCLASS( ClassGroup=(Spaceship), meta=(BlueprintSpawnableComponent) )
class SPATACS_API UFuelTank : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFuelTank();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetCapacity() const;
	float GetCurrent() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Capacity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentFuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PumpRate;
	
private:
	UPROPERTY()
	UDistributionSystemComponent* DistributionSystem;
};
