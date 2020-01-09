// Fill out your copyright notice in the Description page of Project Settings.


#include "SWPMapBase.h"
#include "CoalaAreaController.h"

// Sets default values
ASWPMapBase::ASWPMapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TileList.Empty();
	EastCoastTileList.Empty();
	WestCoastTileList.Empty();
	SouthCoastTileList.Empty();
	NorthCoastTileList.Empty();
	mapExpansionMode = EMapExpansion::None;
}

// Called when the game starts or when spawned
void ASWPMapBase::BeginPlay()
{
	Super::BeginPlay();
	
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

void ASWPMapBase::CreateMapTileDataBase(TArray<FCoalaRemoteTileRequest> newAreasInRange)
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
			CreateMapTileDataBase(TileList);
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
			CreateMapTileDataBase(TileList);
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
			CreateMapTileDataBase(TileList);
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
			CreateMapTileDataBase(TileList);
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
	//SpawnCenteralActor();
}

void ASWPMapBase::SpawnCenteralActor()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const world = GetWorld();
	FVector Location(0, 0, 0);
	FRotator Rotation(0, 0, 0);

	MapCentralActor = world->SpawnActor(AActor::StaticClass(), &Location, &Rotation, SpawnInfo);

	if (MapCentralActor != nullptr)
		UE_LOG(LogFlying, Warning, TEXT("Map Center Actor Spawned Successfully"));

}
