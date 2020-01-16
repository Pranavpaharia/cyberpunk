// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SWPMapBase.h"
#include "TopDownPlayerController.h"
#include "SWPCoalaGameMode.generated.h"

UCLASS(MinimalAPI)
class ASWPCoalaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASWPCoalaGameMode();
	virtual void BeginPlay() override;
	void SpawnMapTiler();
	void GetWorldPawn();


public:
	ASWPCoalaPawn* swpCharacter;
	ASWPMapBase* mapObject;
	double mlat = 0.0f;
	double mlon = 0.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASWPMapBase> mapObjectBP;
};



