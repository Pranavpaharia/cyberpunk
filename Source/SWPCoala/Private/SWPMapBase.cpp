// Fill out your copyright notice in the Description page of Project Settings.


#include "SWPMapBase.h"
#include "CoalaAreaController.h"
#include "BluePrintHttpGetRequest.h"
#include "CoalaMeshGenerator.h"
#include "CoalaReadJsonAsync.h"
#include "TopDownPlayerController.h"
#include "CoalaMeshActor.h"
#include "CoalaBlueprintUtility.h"

class UCoalaAreaController;

// Sets default values
ASWPMapBase::ASWPMapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	defaultBuildingLevel = 3;
	clampToDefaultBuildingLevel = true;
	limitMaxBuildingLevelTo = 0;
	CellZOffSet = -2.0f;

	areaList.Empty();
	TileList.Empty();
	EastCoastTileList.Empty();
	WestCoastTileList.Empty();
	SouthCoastTileList.Empty();
	NorthCoastTileList.Empty();
	mapExpansionMode = EMapExpansion::None;

	bAsyncJson = false;
}

// Called when the game starts or when spawned
void ASWPMapBase::BeginPlay()
{
	Super::BeginPlay();

	//Init Coala Instance
	IntializeCoalaServices();

	//Reset the Work Threads
	ResetAllTaskThreads();

	//Calculate the Tiles needed based on Lat and Lon
	UCoalaAreaController::OnGpsPositionChanged(MapZoom, MapLongitude, MapLatitude, TileList, TileListOutOfRange, Areabuffer);
	
	//Calculate the Tile Extension
	CalculateMapTileExtensions(TileList);

	//Create 
	MapPtrForSuccess.BindUFunction(this, "SuccessfulJsonResponse");
	MapPtrForError.BindUFunction(this, "PrintErrorString");
	
	//Create HTTP Request for each tile
	CreateTileWebRequest(TileList);
}

// Called every frame
void ASWPMapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void ASWPMapBase::ClearTileMapList(bool bResetMainList)
{
	if (bResetMainList)
		TileList.Empty();

	EastCoastTileList.Empty();
	WestCoastTileList.Empty();
	SouthCoastTileList.Empty();
	NorthCoastTileList.Empty();

}

void ASWPMapBase::CalculateMapTileExtensions(TArray<FCoalaRemoteTileRequest> newAreasInRange)
{

	TileList = newAreasInRange;

	int number = 0; number++;
	int tileX = 0;
	int tileY = 0;
	int maxEastValue = TileList[0].tile_y;
	int maxWestValue = TileList[0].tile_y;
	int maxNorthValue = TileList[0].tile_x;
	int maxSouthValue = TileList[0].tile_x;
	//UE_LOG(LogTemp, Warning, TEXT("Tile List has: %d number of elements"), number);
	//Read Tile Information
	for (FCoalaRemoteTileRequest mapTile : TileList)
	{
		tileX = mapTile.tile_x;
		tileY = mapTile.tile_y;
		//UE_LOG(LogTemp, Warning, TEXT("TileX: %d || TileY: %d || Tile Number: %d"), tileX,tileY, number);
		number++;

		if (maxEastValue < mapTile.tile_y)
			maxEastValue = mapTile.tile_y;

		if (maxWestValue > mapTile.tile_y)
			maxWestValue = mapTile.tile_y;

		if (maxNorthValue < mapTile.tile_x)
			maxNorthValue = mapTile.tile_x;

		if (maxSouthValue > mapTile.tile_x)
			maxSouthValue = mapTile.tile_x;

	}

	//UE_LOG(LogTemp, Warning, TEXT("East Coast: %d || West Coast: %d || North Coast: %d || South Coast: %d"), maxEastValue, maxWestValue, maxNorthValue, maxSouthValue);


	for (FCoalaRemoteTileRequest mapTile : TileList)
	{
		//Get EastCoast Tiles	
		if (maxEastValue == mapTile.tile_y)
			EastCoastTileList.Emplace(mapTile);

		//Get West Coast Tiles
		if (maxWestValue == mapTile.tile_y)
			WestCoastTileList.Emplace(mapTile);

		//Get North Coast Tiles
		if (maxNorthValue == mapTile.tile_x)
			NorthCoastTileList.Emplace(mapTile);

		//Get South Coast Tiles
		if (maxSouthValue == mapTile.tile_x)
			SouthCoastTileList.Emplace(mapTile);

	}

	number = EastCoastTileList.Num();
	UE_LOG(LogFlying, Warning, TEXT("East Coast: %d "), number);
	number = WestCoastTileList.Num();
	UE_LOG(LogFlying, Warning, TEXT("West Coast: %d "), number);
	number = SouthCoastTileList.Num();
	UE_LOG(LogFlying, Warning, TEXT("South Coast: %d "), number);
	number = NorthCoastTileList.Num();
	UE_LOG(LogFlying, Warning, TEXT("North Coast: %d "), number);


}

void ASWPMapBase::NeedToExpandMapInDir(EMapExpansion ExpansionMode, TArray<FCoalaRemoteTileRequest>& newTileArray)
{
	switch (ExpansionMode)
	{
		case EMapExpansion::NorthExp:

			for (FCoalaRemoteTileRequest mTile : NorthCoastTileList)
			{
				FCoalaRemoteTileRequest newTile;
				newTile.tile_x = mTile.tile_x + 1;
				newTile.tile_y = mTile.tile_y;
				newTile.zoom = mTile.zoom;
				TileList.Emplace(newTile);
			}
			ClearTileMapList(false);
			CalculateMapTileExtensions(TileList);
			UE_LOG(LogFlying, Warning, TEXT("In North Section"));
			newTileArray = NorthCoastTileList;
			ShowListItems(NorthCoastTileList);
			break;

		case EMapExpansion::SouthExp:

			for (FCoalaRemoteTileRequest mTile : SouthCoastTileList)
			{
				FCoalaRemoteTileRequest newTile;
				newTile.tile_x = mTile.tile_x - 1;
				newTile.tile_y = mTile.tile_y;
				newTile.zoom = mTile.zoom;
				TileList.Emplace(newTile);
			}
			ClearTileMapList(false);
			CalculateMapTileExtensions(TileList);
			UE_LOG(LogFlying, Warning, TEXT("In South Section"));
			newTileArray = SouthCoastTileList;
			ShowListItems(SouthCoastTileList);
			break;

		case EMapExpansion::EastExp:

			for (FCoalaRemoteTileRequest mTile : EastCoastTileList)
			{
				FCoalaRemoteTileRequest newTile;
				newTile.tile_x = mTile.tile_x;
				newTile.tile_y = mTile.tile_y + 1;
				newTile.zoom = mTile.zoom;
				TileList.Emplace(newTile);
			}
			ClearTileMapList(false);
			CalculateMapTileExtensions(TileList);
			newTileArray = EastCoastTileList;
			ShowListItems(EastCoastTileList);
			UE_LOG(LogFlying, Warning, TEXT("In East Section"));
			break;

		case EMapExpansion::WestExp:

			for (FCoalaRemoteTileRequest mTile : WestCoastTileList)
			{
				FCoalaRemoteTileRequest newTile;
				newTile.tile_x = mTile.tile_x;
				newTile.tile_y = mTile.tile_y - 1;
				newTile.zoom = mTile.zoom;
				TileList.Emplace(newTile);
			}
			ClearTileMapList(false);
			CalculateMapTileExtensions(TileList);
			newTileArray = WestCoastTileList;
			ShowListItems(WestCoastTileList);
			UE_LOG(LogFlying, Warning, TEXT("In West Section"));
			break;

		case EMapExpansion::None:
			UE_LOG(LogFlying, Warning, TEXT("In No Expansion Settings"));
			break;
	}
}

void ASWPMapBase::ShowListItems(TArray<FCoalaRemoteTileRequest> mList)
{
	int tileX = 0; int tileY = 0; int number = 0; number++;
	for (FCoalaRemoteTileRequest mTile : mList)
	{
		tileX = mTile.tile_x;
		tileY = mTile.tile_y;
		UE_LOG(LogFlying, Warning, TEXT("TileX: %d || TileY: %d || Tile Number: %d"), tileX, tileY, number);
		number++;
	}
}

void ASWPMapBase::IntializeCoalaServices()
{
	UCoalaAreaController::InitCoala();
	SpawnCenteralActor();
}

void ASWPMapBase::SpawnCenteralActor()
{
	ATopDownPlayerController* pc = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc != nullptr)
	{
		UE_LOG(LogFlying, Warning, TEXT("Found Player Controller Reference"));
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const world = GetWorld();
	FVector Location(0, 0, 0);
	FRotator Rotation(0, 0, 0);

	MapCentralActor = world->SpawnActor(AActor::StaticClass(), &Location, &Rotation, SpawnInfo);

	if (MapCentralActor != nullptr)
	{
		UE_LOG(LogFlying, Warning, TEXT("Map Center Actor Spawned Successfully"));
		//pc->SetAnchorObject(MapCentralActor);
	}
}

void ASWPMapBase::ResetAllTaskThreads()
{
	UCoalaBlueprintUtility::ResetBuildingsMeshThread();
	UCoalaBlueprintUtility::ResetCellMeshThread();
	UCoalaBlueprintUtility::ResetJsonThread();
	UCoalaBlueprintUtility::ResetBuildingsMeshThread();
	UCoalaBlueprintUtility::ResetWaterMeshThread();
	UCoalaBlueprintUtility::ResetStreetsMeshThread();
	UE_LOG(LogFlying, Warning, TEXT("All working threads are Reset"));
}

void ASWPMapBase::SetPlayerCharacterRef(ASWPCoalaPawn* playerCharacter)
{
	PlayerCharacter = playerCharacter;

	if(PlayerCharacter != nullptr)
	{ 
		UE_LOG(LogFlying, Warning, TEXT("We got the Coala Character"));
	//	PlayerCharacter->MapObject = this;
	}
	else
	{
		UE_LOG(LogFlying, Warning, TEXT("We didn't get the Coala Character"));
	}
}

void ASWPMapBase::SuccessfulJsonResponse(FString str)
{
	/*if (areaList == NULL)
	{
		UE_LOG(LogFlying, Warning, TEXT("Area list is Null. Now Returning"));
		return;
	}*/
	
	areaList.Emplace(str);

	if (bAsyncJson)
	{
		UE_LOG(LogFlying, Warning, TEXT("Thread is still working on..."));
		return;

		/*
		if (areaListIndex < areaList.Num())
		{
			FString newAreaString = areaList[areaListIndex];

			UCoalaBlueprintUtility::LoadCoalaAreaFromResponseAsync(newAreaString, 0, 1, 2);
			FCoalaReadJsonAsync::Runnable->JsonRead.BindUFunction(this, "PrintJsonString");

			UE_LOG(LogFlying, Warning, TEXT("Area List is making thread request for %d th. "),areaListIndex+1);
			areaListIndex++;
		}

		if (areaListIndex == areaList.Num())
		{
			areaListIndex = 0;
			UE_LOG(LogFlying, Warning, TEXT("Area List done with all the Indexes"));
		}
		*/
	}
	else
	{
		CoalaAreaSet = UCoalaBlueprintUtility::LoadCoalaAreaFromResponse(str, 
						defaultBuildingLevel, clampToDefaultBuildingLevel,
						limitMaxBuildingLevelTo);

		if (CoalaAreaSet != nullptr)
		{
			UE_LOG(LogFlying, Warning, TEXT("found Coala Area Value "));
			CreateGridCells(CoalaAreaSet);
			CreateWaterMeshes(CoalaAreaSet);
			//CreateAreaDimensions(CoalaAreaSet);
			CreateStreetMeshes(CoalaAreaSet);
			CreateBuildingMeshes(CoalaAreaSet);
			UCoalaAreaController::AddKnownArea(CoalaAreaSet);
		}
	}

	//UCoalaBlueprintUtility::LoadCoalaAreaFromResponseAsync(str, 0, 1, 2);	
	//FCoalaReadJsonAsync::Runnable->JsonRead.BindUFunction(this, "PrintJsonString");
	//bThreadStarted = true;

	/*while (!UCoalaBlueprintUtility::AsyncLoadingDone())
	{
		CoalaAreaSet = UCoalaBlueprintUtility::GetAsyncJsonResult();
		if (CoalaAreaSet != nullptr)
			UE_LOG(LogFlying, Warning, TEXT("found Coala Area Value "));
	}
	do
	{

	}*/
	
	
	
}

void ASWPMapBase::PrintJsonString()
{
	UE_LOG(LogFlying, Warning, TEXT("Json Reading is done "));

	FCoalaReadJsonAsync::Runnable->JsonRead.Unbind();
}


void ASWPMapBase::PrintErrorString(FString str)
{
	UE_LOG(LogFlying, Warning, TEXT("Printing ERROR: %s"), *str);
}

void ASWPMapBase::CreateTileWebRequest(TArray<FCoalaRemoteTileRequest> mapList)
{
	for (FCoalaRemoteTileRequest mTile : mapList)
	{
		EOutputPins_CoalaRequestResult branches;
		UBluePrintHttpGetRequest* Request = UCoalaBlueprintUtility::MakeCoalaRequest(APIKey, mTile, 126, branches);
		Request->OnSuccess.Add(MapPtrForSuccess);
		Request->OnError.Add(MapPtrForError); 
	}
}

void ASWPMapBase::CreateGridCells(UCoalaArea* mCoalaArea)
{
	TArray<UCoalaCell*> cellGrid = mCoalaArea->grid;
	ACoalaMeshActor* cellActor = UCoalaMeshGenerator::CreateCells(MapCentralActor, mCoalaArea, cellGrid, defaultCellRenderConfig, CellRenderConfigList);

	if (cellActor != nullptr)
	{
		//UE_LOG(LogFlying, Warning, TEXT("Cell Actor is Created "));
		FVector newCellPosition = FVector(cellActor->GetActorLocation().X, cellActor->GetActorLocation().Y, CellZOffSet);
		cellActor->SetActorLocation(newCellPosition, false, nullptr, ETeleportType::None);
		//UCoalaAreaController::AddKnownArea(mCoalaArea);
	}
}

void ASWPMapBase::CreateAreaDimensions(UCoalaArea* mCoalaArea)
{
	ACoalaMeshActor* CoalaAreaMesh = UCoalaMeshGenerator::CreateAreaDimensions(MapCentralActor, mCoalaArea, AreaMaterial);

	if (CoalaAreaMesh != nullptr)
	{
		//UE_LOG(LogFlying, Warning, TEXT("Cell Actor is Created "));
		FVector newCellPosition = FVector(CoalaAreaMesh->GetActorLocation().X, CoalaAreaMesh->GetActorLocation().Y, CellZOffSet);
		CoalaAreaMesh->SetActorLocation(newCellPosition, false, nullptr, ETeleportType::None);
		//UCoalaAreaController::AddKnownArea(mCoalaArea);
	}
}

void ASWPMapBase::CreateWaterMeshes(UCoalaArea* mCoalaArea)
{
	TArray<UCoalaWater*> waterGrid = mCoalaArea->water;

	for (UCoalaWater* waterCell : waterGrid)
	{
		ACoalaMeshActor* waterMesh = UCoalaMeshGenerator::CreateWater(MapCentralActor, mCoalaArea, waterCell, WaterMaterial, true, WaterOutlineWidth, WaterOutlineMaterial);

		if (waterMesh != nullptr)
		{
			//UE_LOG(LogFlying, Warning, TEXT("Cell Actor is Created "));
			FVector newCellPosition = FVector(waterMesh->GetActorLocation().X, waterMesh->GetActorLocation().Y, CellZOffSet+1);
			waterMesh->SetActorLocation(newCellPosition, false, nullptr, ETeleportType::None);
		}
	}

	//UCoalaAreaController::AddKnownArea(mCoalaArea);
}

void ASWPMapBase::CreateStreetMeshes(UCoalaArea* mCoalaArea)
{
	TArray<UCoalaStreets*> streetsGrid = mCoalaArea->streets;

	for (UCoalaStreets* StreetCell : streetsGrid)
	{
		ACoalaMeshActor* StreetMesh = UCoalaMeshGenerator::CreateStreets(MapCentralActor, mCoalaArea, StreetCell, defaultStreetRenderConfig, StreetRenderConfigList); 
		
		if (StreetMesh != nullptr)
		{
			FVector newCellPosition = FVector(StreetMesh->GetActorLocation().X, StreetMesh->GetActorLocation().Y, CellZOffSet);
			StreetMesh->SetActorLocation(newCellPosition, false, nullptr, ETeleportType::None);
		}
	}

	//UCoalaAreaController::AddKnownArea(mCoalaArea);
}


void ASWPMapBase::CreateBuildingMeshes(UCoalaArea* mCoalaArea)
{
	// We are making the buildings and combining them
	TArray<UCoalaBuilding*> buildingGrid = mCoalaArea->buildings;

	ACoalaMeshActor* BuildingMesh = UCoalaMeshGenerator::CreateBuildings(MapCentralActor, mCoalaArea, buildingGrid, BuildingMeshes, BuildingFloorMaterial, BuildingWallMaterial, BuildingRoofMaterial, GenerateBuildingUV, BuildingHeightPerLevel, GenerateCollisionForBuildings);

	if (BuildingMesh != nullptr)
	{
		FVector newCellPosition = FVector(BuildingMesh->GetActorLocation().X, BuildingMesh->GetActorLocation().Y, CellZOffSet);
		BuildingMesh->SetActorLocation(newCellPosition, false, nullptr, ETeleportType::None);
	}
	
}


