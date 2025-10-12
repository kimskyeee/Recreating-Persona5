// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuConfigCam.h"


// Sets default values
AMenuConfigCam::AMenuConfigCam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMenuConfigCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuConfigCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

