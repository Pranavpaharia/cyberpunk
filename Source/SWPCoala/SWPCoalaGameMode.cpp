// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SWPCoalaGameMode.h"
#include "SWPCoalaPawn.h"

ASWPCoalaGameMode::ASWPCoalaGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ASWPCoalaPawn::StaticClass();
}
