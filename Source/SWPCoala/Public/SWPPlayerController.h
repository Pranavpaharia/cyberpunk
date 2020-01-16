// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownPlayerController.h"
#include "SWPCoalaPawn.h"
#include "SWPMapBase.h"
#include "SWPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SWPCOALA_API ASWPPlayerController : public ATopDownPlayerController
{
	GENERATED_BODY()
	ASWPPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void CheckDistance(bool bCheck);
	

public:
	void SetAnchorObject(ASWPMapBase* anchorObj);


public:
	ASWPMapBase* mapAnchorObject;
	ASWPCoalaPawn* playerObject;
	bool bStartDistanceCheck;
	UActorComponent* playerComponent;

	float distanceEWAxisMaxRange;
	bool bEWDistanceCheck;

	float distanceNSAxisMaxRange;
	bool bNSDistanceCheck;

	int deltaIncreaseRange;


	
};
