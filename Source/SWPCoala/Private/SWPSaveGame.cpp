// Fill out your copyright notice in the Description page of Project Settings.


#include "SWPSaveGame.h"

USWPSaveGame::USWPSaveGame()
{
	PlayerName = TEXT("Player1");
	SaveSlotName = TEXT("A1");
	TileInfoList.Reserve(20);
}

FString USWPSaveGame::ParseResponse(FString str)
{
	FString responseStr;

	jsonObject = MakeShareable(new FJsonObject());

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(str);

	if (FJsonSerializer::Deserialize(JsonReader, jsonObject) && jsonObject.IsValid())
	{
		TSharedPtr<FJsonObject> ResultObject = jsonObject->GetObjectField("props");

		ResultObject = ResultObject->GetObjectField("tile");

		responseStr = FString::FromInt(ResultObject->GetIntegerField("x")) + TEXT("X /");
		responseStr = responseStr + FString::FromInt(ResultObject->GetIntegerField("y")) + TEXT("Y /");
		responseStr = responseStr + FString::FromInt(ResultObject->GetIntegerField("z")) + TEXT("Z");
	}

	return responseStr;
}