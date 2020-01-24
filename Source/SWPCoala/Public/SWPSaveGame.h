// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "SWPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SWPCOALA_API USWPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	TMap<FString,FString> TileInfoList;

	TSharedPtr<FJsonObject> jsonObject;

public:
	USWPSaveGame();

	FString ParseResponse(FString str);
	
};
