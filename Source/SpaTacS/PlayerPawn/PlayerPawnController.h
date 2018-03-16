// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerPawnController.generated.h"


class APlayerPawn;
class FViewport;
class AStarShip;


/**
 * 
 */
UCLASS()
class SPATACS_API APlayerPawnController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerPawnController();

	UFUNCTION(BlueprintPure)
	FVector GetTargetPosition() const;

protected:
	// Begin PlayerController interface
	void PlayerTick(float DeltaTime) override;
	void SetupInputComponent() override;
	// End PlayerController interface

	// Input Handlers
	// Movement Axis
	void MoveForwardAxis(float Amount);
	void MoveSideAxis(float Amount);
	void MoveVerticalAxis(float Amount);

	// Looking Around
	void StartLookAround();
	void EndLookAround();
	void OnMoveMouseX(float Amount);
	void OnMoveMouseY(float Amount);

	void YawAxis(float Amount);
	void RollAxis(float Amount);

	// Unit selection
	void OnClickSelect();

	// Unit Commands
	void OnChangeTargetRange(float Amount);

	APlayerPawn* GetPlayerPawn();
	FViewport* GetViewport();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
	AStarShip* SelectedShip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetZRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReferencePlaneZ;

private:
	bool bLookAroundMode;

	int32 MouseResetX;
	int32 MouseResetY;

	float TargetPositionZ;
};
