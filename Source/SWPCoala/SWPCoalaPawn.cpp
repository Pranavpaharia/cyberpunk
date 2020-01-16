// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SWPCoalaPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

ASWPCoalaPawn::ASWPCoalaPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	//PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	//PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	//RootComponent = PlaneMesh;

	//PlayerHookComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PlayrePositionHook"));
	//PlayerHookComponent->SetupAttachment(CapsuleComponent);

	//// Create a spring arm component
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	//SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	//SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	//SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	//SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	//SpringArm->CameraLagSpeed = 15.f;

	//// Create camera component 
	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	//Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
}

void ASWPCoalaPawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	//// Move plan forwards (with sweep so we stop when we collide with things)
	//AddActorLocalOffset(LocalMove, true);

	//// Calculate change in rotation this frame
	//FRotator DeltaRotation(0,0,0);
	//DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	//DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	//DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	//// Rotate plane
	//AddActorLocalRotation(DeltaRotation);

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void ASWPCoalaPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void ASWPCoalaPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	/*PlayerInputComponent->BindAxis("Thrust", this, &ASWPCoalaPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASWPCoalaPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASWPCoalaPawn::MoveRightInput);*/

	Super::SetupInputComponent();
}

void ASWPCoalaPawn::ThrustInput(float Val)
{
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ASWPCoalaPawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASWPCoalaPawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

EMapExpansion ASWPCoalaPawn::GetExpansionDirection()
{
	EMapExpansion mapDir = EMapExpansion::None;

	switch (PlayerDir)
	{
		case EPlayerDirEnum::PD_North:
		{
			mapDir = EMapExpansion::NorthExp;
			break;
		}

		case EPlayerDirEnum::PD_South:
		{
			mapDir = EMapExpansion::SouthExp;
			break;
		}

		case EPlayerDirEnum::PD_East:
		{
			mapDir = EMapExpansion::EastExp;
			break;
		}

		case EPlayerDirEnum::PD_West:
		{
			mapDir = EMapExpansion::WestExp;
			break;
		}
	}

	return mapDir;
}

