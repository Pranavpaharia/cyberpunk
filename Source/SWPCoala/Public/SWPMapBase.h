// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWPCoala.h"
#include "ThoughtfishCoalaPlugin/Public/CoalaRemoteTileRequest.h"
#include "SWPMapBase.generated.h"


UENUM(BlueprintType)
enum class EChooseCity : uint8
{
	LondonBridge,
	StatueOfLiberty,
	GatewayOfIndia,
	Dubai,
	None
};

UENUM(BlueprintType)
enum class EMapExpansion : uint8
{
	NorthExp,
	SouthExp,
	EastExp,
	WestExp,
	None
};

UCLASS()
class SWPCOALA_API ASWPMapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWPMapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void ClearTileMapList(bool bResetMainList);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void CreateMapTileDataBase(TArray<FCoalaRemoteTileRequest> newAreasInRange);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void NeedToExpandMapInDir(EMapExpansion ExpansionMode, TArray<FCoalaRemoteTileRequest>& newTileArray);

	void ShowListItems(TArray<FCoalaRemoteTileRequest> mList);

	void IntializeCoalaServices();

	void SpawnCenteralActor();


public:
	//This is for member variables
	FString APIKey = TEXT("5F799927B35D0544");
	float mapScale = 1.0f;
	double MapLatitude = 0f;
	double MapLongitude = 0f;
	AActor* MapCentralActor;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
		TArray<FCoalaRemoteTileRequest> TileList;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
		TArray<FCoalaRemoteTileRequest> EastCoastTileList;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
		TArray<FCoalaRemoteTileRequest> WestCoastTileList;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
		TArray<FCoalaRemoteTileRequest> NorthCoastTileList;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
		TArray<FCoalaRemoteTileRequest> SouthCoastTileList;

	UPROPERTY(BlueprintReadWrite, Category = "MapTiles")
	EMapExpansion mapExpansionMode;




};
