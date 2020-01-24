// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugUserInterface.h"
#include "SWPCoala.h"


void UDebugUserInterface::NativeOnInitialized()
{
	//Heading1 = NSLOCTEXT("Example Text", "Example Text2", "Example Text3");
	//return true;
	UE_LOG(LogFlying, Warning, TEXT("Debug UI Initialized "));
	Heading1 = TEXT("Demo HUD Text !");
}

void UDebugUserInterface::NativeConstruct()
{

}

