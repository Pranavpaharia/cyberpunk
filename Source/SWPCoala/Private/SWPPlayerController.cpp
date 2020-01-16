// Fill out your copyright notice in the Description page of Project Settings.


#include "SWPPlayerController.h"

ASWPPlayerController::ASWPPlayerController()
{
	deltaIncreaseRange = 8000;
	distanceEWAxisMaxRange = 7000;
	distanceNSAxisMaxRange = 7000;
}

void ASWPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	playerObject = Cast<ASWPCoalaPawn>(GetPawn());
	//playerComponent = Cast<UActorComponent>(playerObject->CapsuleComponent);
	if(playerObject != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Got Player reference in Player Controller"))
}


void ASWPPlayerController::CheckDistance(bool bCheck)
{
	if (bCheck)
	{
		FVector PositionOrigin = mapAnchorObject->GetActorLocation();
		FVector CharacterPosition = playerObject->GetActorLocation();

		FVector PositionOriginNSAxis = FVector(mapAnchorObject->GetActorLocation().X, 0, 0);
		FVector PositionOriginEWAxis = FVector(0, mapAnchorObject->GetActorLocation().Y, 0);

		FVector PositionCharacterNSAxis = FVector(playerObject->GetActorLocation().X, 0, 0);
		FVector PositionCharacterEWAxis = FVector(0, playerObject->GetActorLocation().Y, 0);

		float distanceEWAxis = FVector::Dist(PositionOriginEWAxis, PositionCharacterEWAxis);
		float distanceNSAxis = FVector::Dist(PositionOriginNSAxis, PositionCharacterNSAxis);
		//float distanceIn = FVector::Dist2D(PositionOrigin, CharacterPosition);

		//UE_LOG(LogTemp, Warning, TEXT("Distance on DistanceEWAxis: %f || DistanceNSAxis: %f"), distanceEWAxis, distanceNSAxis);
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %f meters"), distanceIn);
	
		if (distanceEWAxis > distanceEWAxisMaxRange)
		{
			TArray<FCoalaRemoteTileRequest> newTileArray;
			distanceEWAxisMaxRange += 2000;
			mapAnchorObject->CalculateMapTileExtensions(mapAnchorObject->TileList);
			mapAnchorObject->NeedToExpandMapInDir(playerObject->GetExpansionDirection(), newTileArray);
			mapAnchorObject->CreateTileWebRequest(newTileArray);
		}

		if (distanceNSAxis > distanceNSAxisMaxRange)
		{
			TArray<FCoalaRemoteTileRequest> newTileArray;
			distanceNSAxisMaxRange += deltaIncreaseRange;
			mapAnchorObject->CalculateMapTileExtensions(mapAnchorObject->TileList);
			mapAnchorObject->NeedToExpandMapInDir(playerObject->GetExpansionDirection(), newTileArray);
			mapAnchorObject->CreateTileWebRequest(newTileArray);
		}

	}
}

void ASWPPlayerController::SetAnchorObject(ASWPMapBase* anchorObj)
{
	if (anchorObj != nullptr)
	{
		mapAnchorObject = anchorObj;
		bStartDistanceCheck = true;
	}

}

void ASWPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckDistance(bStartDistanceCheck);
}
