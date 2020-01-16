// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWPCoala.h"
#include "CoalaMeshGenerator.h"
#include "CoalaArea.h"
#include "SWPCoalaPawn.h"
#include "ThoughtfishCoalaPlugin/Public/CoalaRemoteTileRequest.h"
#include "SWPMapBase.generated.h"


USTRUCT(BlueprintType)
struct FMapVisibilityRange
{
	GENERATED_BODY();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapRange)
	int NorthRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapRange)
	int SouthRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapRange)
	int EastRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapRange)
	int WestRange;

public:
	FMapVisibilityRange()
	{
		NorthRange = 10;
		SouthRange = NorthRange;
		EastRange = NorthRange;
		WestRange = NorthRange;
	}

	void ExtendNorthRange(int extendedRange)
	{
		NorthRange = NorthRange + extendedRange;
	}

	void ExtendSouthRange(int extendedRange)
	{
		SouthRange = SouthRange + extendedRange;
	}

	void ExtendEastRange(int extendedRange)
	{
		EastRange = EastRange + extendedRange;
	}

	void ExtendWestRange(int extendedRange)
	{
		WestRange = WestRange + extendedRange;
	}

	int GetNorthRange()
	{
		return NorthRange;
	}

	int GetSouthRange()
	{
		return SouthRange;
	}

	int GetEastRange()
	{
		return EastRange;
	}

	int GetWestRange()
	{
		return WestRange;
	}
};



UCLASS()
class SWPCOALA_API ASWPMapBase : public AActor, public TSharedFromThis<ASWPMapBase>
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
	void CalculateMapTileExtensions(TArray<FCoalaRemoteTileRequest> newAreasInRange);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void NeedToExpandMapInDir(EMapExpansion ExpansionMode, TArray<FCoalaRemoteTileRequest>& newTileArray);

	void ShowListItems(TArray<FCoalaRemoteTileRequest> mList);

	void IntializeCoalaServices();

	void SpawnCenteralActor();

	void ResetAllTaskThreads();

	void SetPlayerCharacterRef(ASWPCoalaPawn* playerCharacter);

	void CreateTileWebRequest(TArray<FCoalaRemoteTileRequest> mapList);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void SuccessfulJsonResponse(FString str);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void PrintErrorString(FString str);

	UFUNCTION(BlueprintCallable, Category = "MapTiles")
	void PrintJsonString();

	UFUNCTION(BlueprintCallable, Category = "GridCells")
	void CreateGridCells(UCoalaArea* mCoalaArea);

	UFUNCTION(BlueprintCallable, Category = "CoalaArea")
	void CreateAreaDimensions(UCoalaArea* mCoalaArea);

	UFUNCTION(BlueprintCallable, Category = "Water")
	void CreateWaterMeshes(UCoalaArea* mCoalaArea);

	UFUNCTION(BlueprintCallable, Category = "Streets")
	void CreateStreetMeshes(UCoalaArea* mCoalaArea);

	UFUNCTION(BlueprintCallable, Category = "Streets")
	void CreateBuildingMeshes(UCoalaArea* mCoalaArea);

public:
	//This is for member variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapTiles")
	FString APIKey = TEXT("5F799927B35D0544");
	
	UCoalaArea* CoalaAreaSet;
	int areaListIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	bool bAsyncJson;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	int Areabuffer = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	float mapScale = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCharacter")
	uint8 MapZoom = 14;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCharacter")
	float MapLatitude = 52.499531f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCharacter")
	float MapLongitude = 13.295502f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	int defaultBuildingLevel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	bool clampToDefaultBuildingLevel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	int limitMaxBuildingLevelTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCharacter")
	AActor* MapCentralActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridCells")
	FCoalaCellRenderConfig defaultCellRenderConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridCells")
	TArray<FCoalaCellRenderConfig> CellRenderConfigList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FString> areaList;

	TScriptDelegate<FWeakObjectPtr> MapPtrForSuccess, MapPtrForError;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCharacter")
	ASWPCoalaPawn* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> TileList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> TileListOutOfRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> EastCoastTileList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> WestCoastTileList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> NorthCoastTileList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	TArray<FCoalaRemoteTileRequest> SouthCoastTileList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapTiles")
	EMapExpansion mapExpansionMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapTiles")
	float CellZOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapTiles")
	FMapVisibilityRange MapVisibilityRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoalaArea")
	UMaterialInterface* AreaMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
	UMaterialInterface* WaterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
	UMaterialInterface* WaterOutlineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
	float WaterOutlineWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
	bool GenerateWaterUV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streets")
	FCoalaStreetRenderConfig defaultStreetRenderConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streets")
	TArray<FCoalaStreetRenderConfig> StreetRenderConfigList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UMaterialInterface* BuildingFloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UMaterialInterface* BuildingWallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UMaterialInterface* BuildingRoofMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building", Meta = (Bitmask, Bitflags, UseEnumValuesAsMaskValuesInEditor = "true", BitmaskEnum = "OPTIONS_MESH_CREATION_BUILDING"))
	int32 BuildingMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float BuildingHeightPerLevel = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	bool GenerateBuildingUV = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	bool CombineBuildingMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	bool GenerateCollisionForBuildings = true;


};
