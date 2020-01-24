// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugUserInterface.generated.h"

/**
 * 
 */
class FRegistryObject;

UCLASS(BlueprintType)
class SWPCOALA_API UDebugUserInterface : public UUserWidget, public TSharedFromThis<UDebugUserInterface>
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User Widget")
	FString Heading1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User Widget")
	FString Heading2;

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

//	virtual void Register(const TCHAR* PackageName, const TCHAR* Name) override;
};
