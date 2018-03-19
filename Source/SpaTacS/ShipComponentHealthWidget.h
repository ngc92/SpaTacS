// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarShip.h"
#include "Blueprint/UserWidget.h"
#include "ShipComponentHealthWidget.generated.h"

class UProgressBar;
class UShipComponentBase;

/**
 * 
 */
UCLASS()
class SPATACS_API UShipComponentHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetShip(const AStarShip* Ship);

    void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UPROPERTY(BlueprintReadOnly)
	const AStarShip* Ship;
	
private:
	UPROPERTY()
	TArray<UShipComponentBase*> SubComps;

	UPROPERTY()
	TArray<UProgressBar*> HealthBars;
};
