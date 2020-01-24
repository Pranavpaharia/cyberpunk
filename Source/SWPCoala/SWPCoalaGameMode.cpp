// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SWPCoalaGameMode.h"
#include "SWPCoala.h"
#include "CoalaCharacterBase.h"
#include "SWPPlayerController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "SWPCoalaPawn.h"

ASWPCoalaGameMode::ASWPCoalaGameMode()
{
	// set default pawn class to our flying pawn
	//DefaultPawnClass = ASWPCoalaPawn::StaticClass();
	
	ConstructorHelpers::FObjectFinder<UBlueprint> iaWidgetClass(TEXT("WidgetBlueprint'/Game/Flying/Utilities/DebugMenu.DebugMenu'"));

	if (iaWidgetClass.Object)
	{
		DebugWidgetBP = (UClass*)iaWidgetClass.Object->GeneratedClass;
	}

}

void ASWPCoalaGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldPawn();

	//TSubclassOf<UUserWidget>* widget1 = LoadDebugUIFromPath(TEXT("D:/Projects/SWPCoala/Content/Flying/Utilities/DebugMenu.uasset"));
	
	if (DebugWidgetBP != nullptr)
	{
		debugMenu = Cast<UDebugUserInterface>(CreateWidget(GetWorld()->GetFirstPlayerController(), DebugWidgetBP, TEXT("DEBUGSCREEN")));
		debugMenu->AddToViewport(0);
	}

	ASWPPlayerController* pC = Cast<ASWPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pC != nullptr)
	{
		pC->SetAnchorObject(mapObject);
		pC->DebugUI = debugMenu;
	}

}

void ASWPCoalaGameMode::GetWorldPawn()
{
	const UWorld* world = GetWorld();

	if (world != nullptr)
	{
		ASWPPlayerController* pC = Cast<ASWPPlayerController>(world->GetFirstPlayerController());


		if (pC != nullptr)
		{
			APawn* achar = pC->GetPawn();
			swpCharacter = Cast<ASWPCoalaPawn>(achar);

			if (swpCharacter != nullptr)
				UE_LOG(LogFlying, Warning, TEXT("In GameMode Got Character Reference"));

			//UE_LOG(LogFlying, Warning, TEXT("Game Mode Begin Play Called"));
			SpawnMapTiler();
		}

	}
}

void ASWPCoalaGameMode::SpawnMapTiler()
{
	const UWorld* world = GetWorld();

	FVector const Location = FVector::ZeroVector;
	FRotator const Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mapObject = GetWorld()->SpawnActor<ASWPMapBase>(mapObjectBP, Location, Rotation, SpawnInfo);
	mapObject->SetPlayerCharacterRef(swpCharacter);
	swpCharacter->mapObject = mapObject;

	
}
