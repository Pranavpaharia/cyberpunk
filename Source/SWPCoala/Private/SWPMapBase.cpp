// Fill out your copyright notice in the Description page of Project Settings.


#include "SWPMapBase.h"

// Sets default values
ASWPMapBase::ASWPMapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

