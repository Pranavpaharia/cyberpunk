// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWPSaveGame.h"
#include "Engine/GameInstance.h"
#include "SWPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWPCOALA_API USWPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	USWPGameInstance();

	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
	USWPSaveGame* mSaveGame;
	
};
