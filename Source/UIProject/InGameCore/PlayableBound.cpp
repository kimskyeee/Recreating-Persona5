// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableBound.h"

#include "Components/BoxComponent.h"


// Sets default values
APlayableBound::APlayableBound()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	SetRootComponent(Bounds);
	Bounds->SetBoxExtent(FVector(3200,2200,700));
}

// Called when the game starts or when spawned
void APlayableBound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayableBound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayableBound::GetWorldXYMinMax(FVector2D& OutMin, FVector2D& OutMax) const
{
	const FVector Origin = GetActorLocation();
	const FVector Extend = Bounds->GetScaledBoxExtent();
	OutMin = FVector2D(Origin.X - Extend.X, Origin.Y - Extend.Y);
	OutMax = FVector2D(Origin.X + Extend.X, Origin.Y + Extend.Y);
}

