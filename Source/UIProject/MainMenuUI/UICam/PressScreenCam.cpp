// Fill out your copyright notice in the Description page of Project Settings.


#include "PressScreenCam.h"


// Sets default values
APressScreenCam::APressScreenCam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APressScreenCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APressScreenCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

