// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShipComponentBase.h"
#include "MagazineComponent.generated.h"


USTRUCT(BlueprintType)
struct SPATACS_API FStoredItem
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    UClass* Type;

    UPROPERTY(EditAnywhere)
    int32 Amount;
};



UCLASS(ClassGroup = (Spaceship), meta = (BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Physics, Collision))
class SPATACS_API UMagazineComponent : public UShipComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMagazineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
    int GetFromStorage(const UClass* DesiredType, int Amount);

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FStoredItem> StoredItems;
};
