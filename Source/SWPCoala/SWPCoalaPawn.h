// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SWPCoala.h"
#include "CoalaCharacterBase.h"
#include "SWPCoalaPawn.generated.h"



//struct EMapExpansion;

UCLASS(Config=Game)
class ASWPCoalaPawn : public ACoalaCharacterBase
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	///** Spring arm that will offset the camera */
	//UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* SpringArm;

	///** Camera component that will be our viewpoint */
	//UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* Camera;
public:

	/*UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* PlayerHookComponent;*/

	ASWPCoalaPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides
	
protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);

	void ZoomSpringArm(float Val);

public:

	/** How quickly forward speed changes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Plane)
	int32 Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Plane)
	int32 TurnSpeed;

	/** Max forward speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pitch)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Yaw)
	float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	/** Current roll speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerDir)
	float SpringLegnthDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerDir)
	EPlayerDirEnum PlayerDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerDir)
	AActor* mapObject;

public:
	EMapExpansion GetExpansionDirection();
	/** Returns PlaneMesh subobject **/
	//FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	///** Returns SpringArm subobject **/
	//FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	///** Returns Camera subobject **/
	//FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
