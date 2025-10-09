// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuNewGameCam.h"


// Sets default values
AMenuNewGameCam::AMenuNewGameCam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMenuNewGameCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuNewGameCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

