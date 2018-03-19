// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShipComponentBase.generated.h"


UCLASS( ClassGroup=(Spaceship), meta=(BlueprintSpawnableComponent) )
class SPATACS_API UShipComponentBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaximumHealth() const;

	UFUNCTION(BlueprintPure)
	float GetHealthStatus() const;

private:
	UPROPERTY(EditAnywhere)
	FText ComponentName;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaximumHealth;

};
