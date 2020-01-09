// Copyright Thoughtfish GmbH, 2019
// http://www.thoughtfish.de

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoalaRequests.generated.h"

/**
 * 
 */
UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UCoalaRequestBase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UCoalaRequest : public UCoalaRequestBase
{
	GENERATED_BODY()
	
};

UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UGolfcourseRequest : public UCoalaRequestBase
{
	GENERATED_BODY()
	
};

UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UVectorTileRequest : public UCoalaRequestBase
{
	GENERATED_BODY()

};

UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UWeatherRequest : public UCoalaRequestBase
{
	GENERATED_BODY()

};

UCLASS()
class THOUGHTFISHCOALAPLUGIN_API UGeofencingRequest : public UCoalaRequestBase
{
	GENERATED_BODY()

};
