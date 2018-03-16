// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HullComponent.generated.h"


UCLASS( ClassGroup=(Ship), meta=(BlueprintSpawnableComponent) )
class SPATACS_API UHullComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHullComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float MaximumHealth;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float Mass;
};
