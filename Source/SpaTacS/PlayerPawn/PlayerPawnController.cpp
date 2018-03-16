// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnController.h"
#include "PlayerPawn.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

#include "StarShip.h"
#include "ShipSystems/PropulsionControlComponent.h"

APlayerPawnController::APlayerPawnController()
{
	bShowMouseCursor = true;

	InputYawScale = 45.f;
	InputRollScale = 45.f;
	InputPitchScale = 60.f;
	TargetPositionZ = 0.f;
}


void APlayerPawnController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerPawnController::MoveForwardAxis);
	InputComponent->BindAxis("MoveSideways", this, &APlayerPawnController::MoveSideAxis);
	InputComponent->BindAxis("MoveUpward", this, &APlayerPawnController::MoveVerticalAxis);
	InputComponent->BindAxis("Turn", this, &APlayerPawnController::YawAxis);
	InputComponent->BindAxis("Roll", this, &APlayerPawnController::RollAxis);
	InputComponent->BindAction("ToggleMouseMode", EInputEvent::IE_Pressed, this, &APlayerPawnController::StartLookAround);
	InputComponent->BindAction("ToggleMouseMode", EInputEvent::IE_Released, this, &APlayerPawnController::EndLookAround);
	InputComponent->BindAxis("MouseX", this, &APlayerPawnController::OnMoveMouseX);
	InputComponent->BindAxis("MouseY", this, &APlayerPawnController::OnMoveMouseY);
	InputComponent->BindAction("ClickSelect", EInputEvent::IE_Pressed, this, &APlayerPawnController::OnClickSelect);
	InputComponent->BindAxis("TargetDistance", this, &APlayerPawnController::OnChangeTargetRange);

	/// \todo figure out why this does not seem to work from the construcotr.
	InputYawScale = 45.f;
	InputRollScale = 45.f;
	InputPitchScale = 90.f;

	TargetPositionZ = 0.f;
	TargetZRate = 100.f;
	ReferencePlaneZ = -200.f;
}

APlayerPawn* APlayerPawnController::GetPlayerPawn()
{
	APawn* Pawn = GetPawn();
	return Cast<APlayerPawn>(Pawn);
}

FViewport* APlayerPawnController::GetViewport()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer)
	{
		return LocalPlayer->ViewportClient->Viewport;
	}
	return nullptr;
}


void APlayerPawnController::MoveForwardAxis(float Value)
{
	APlayerPawn* Pawn = GetPlayerPawn();
	if (Pawn)
	{
		Pawn->MoveForward(Value);
	}
}

void APlayerPawnController::MoveSideAxis(float Value)
{
	APlayerPawn* Pawn = GetPlayerPawn();
	if (Pawn)
	{
		Pawn->MoveRight(Value);
	}
}

void APlayerPawnController::MoveVerticalAxis(float Value)
{
	APlayerPawn* Pawn = GetPlayerPawn();
	if (Pawn)
	{
		Pawn->MoveUp(Value);
	}
}

void APlayerPawnController::StartLookAround()
{
	bLookAroundMode = true;
	bShowMouseCursor = false;

	// we need to get the mouse position ourselves here as GetMousePosition return values in [0, 1]
	FViewport* Viewport = GetViewport();
	if (Viewport)
	{
		MouseResetX = Viewport->GetMouseX();
		MouseResetY = Viewport->GetMouseY();
	}
}

void APlayerPawnController::EndLookAround()
{
	bLookAroundMode = false;
	bShowMouseCursor = true;
}

void APlayerPawnController::OnMoveMouseX(float Amount)
{
	if (bLookAroundMode)
	{
		SetMouseLocation(MouseResetX, MouseResetY);
		YawAxis(Amount);
	}
}

void APlayerPawnController::OnMoveMouseY(float Amount)
{
	if (bLookAroundMode)
	{
		SetMouseLocation(MouseResetX, MouseResetY);
		AddPitchInput(Amount * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	}
}

void APlayerPawnController::YawAxis(float Amount)
{
	if (Amount != 0.f)
	{
		///! \todo figure out correct rotation
		/*const FQuat ControlRotation = GetControlRotation().Quaternion();
		const FQuat RotationDirection = { FVector{0.f, 0.f, 1.f}, 1.f *  3.1415f / 180.f };
		const FQuat TransformedRotation = ControlRotation * RotationDirection * ControlRotation.Inverse();
		//FQuat Rot(FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Z), );
		FRotator DesiredRotation = InputYawScale * Amount * GetWorld()->GetDeltaSeconds() * CustomTimeDilation * TransformedRotation.Rotator();
		UE_LOG(LogTemp, Warning, TEXT("%s %s"), *DesiredRotation.ToString(), *ControlRotation.GetUpVector().ToString());
		RotationInput += DesiredRotation;*/
		AddYawInput(Amount * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	}
}

void APlayerPawnController::RollAxis(float Amount)
{
	if (Amount != 0.f)
	{
		//AddRollInput(Amount * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	}
}

void APlayerPawnController::OnChangeTargetRange(float Amount)
{
	TargetPositionZ += TargetZRate * Amount; // *GetWorld()->GetDeltaSeconds();
}

void APlayerPawnController::OnClickSelect()
{
	FHitResult HitResult;
	HitResult.Init();
	/// \todo why complex here? We just want the spehre collision thing.
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, HitResult);
	AActor* Found = HitResult.GetActor();
	if (Found)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clicked Something"));
	}
	if (Found && Found->GetClass()->IsChildOf(AStarShip::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SELECT"));
		SelectedShip = Cast<AStarShip>(Found);
	}
	else {
		if (SelectedShip)
		{
			SelectedShip->GetPropulsion()->DesiredPosition = GetTargetPosition();
		}
	}
}

FVector APlayerPawnController::GetTargetPosition() const
{
	FVector Direction;
	FVector Position;
	DeprojectMousePositionToWorld(Position, Direction);
	float plane_z = ReferencePlaneZ + GetPawn()->GetActorLocation().Z;
	float t = (plane_z - Position.Z) / Direction.Z;
	if (t < 0 || t > 5000)
	{
		t = 5000;
		FVector Candidate = Position + t * Direction + FVector{ 0.f, 0.f, TargetPositionZ };
		return FVector{ Candidate.X, Candidate.Y, plane_z + TargetPositionZ };
	}
	else
	{
		return Position + t * Direction + FVector{ 0.f, 0.f, TargetPositionZ };
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *(Position + t * Direction).ToString());

	
}