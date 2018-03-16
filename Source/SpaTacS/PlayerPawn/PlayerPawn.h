// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UInputComponent;
class UPlayerPawnMovement;
class USphereComponent;

/**
 * 
 */
UCLASS()
class SPATACS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	APlayerPawn();

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	UPawnMovementComponent* GetMovementComponent() const override;

	/// It is possible to couple the camera pawn to a specific actor. The camera will then stay stationary
	/// relative to that actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const AActor* CoupledActor;

	/** Returns CollisionComponent subobject **/
	USphereComponent* GetCollisionComponent() const { return CollisionComponent; }

	// Stuff taken directly from DefaultPawn

	/**
	* Input callback to move forward in local space (or backward if Val is negative).
	* @param Val Amount of movement in the forward direction.
	* @see APawn::AddMovementInput()
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void MoveForward(float Val);

	/**
	* Input callback to strafe right in local space (or left if Val is negative).
	* @param Val Amount of movement in the right direction.
	* @see APawn::AddMovementInput()
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void MoveRight(float Val);

	/**
	* Input callback to move up in local space (or down if Val is negative).
	* @param Val Amount of movement in the up direction.
	* @see APawn::AddMovementInput()
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn")
	virtual void MoveUp(float Val);

private:
	/** DefaultPawn movement component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPlayerPawnMovement* MovementComponent;

	/** DefaultPawn collision component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/// SceneComponent that is used to visually represent the current target position.
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* TargetMarker;
};
