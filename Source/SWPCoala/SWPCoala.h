// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFlying, Log, All);


UENUM(BlueprintType)
enum class EPlayerDirEnum : uint8
{
	PD_North UMETA(DisplayName = "Player North"),
	PD_East UMETA(DisplayName = "Player East"),
	PD_South UMETA(DisplayName = "Player South"),
	PD_West UMETA(DisplayName = "Player West")
};

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