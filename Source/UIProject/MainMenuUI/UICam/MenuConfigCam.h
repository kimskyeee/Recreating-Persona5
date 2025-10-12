// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MenuConfigCam.generated.h"

UCLASS()
class UIPROJECT_API AMenuConfigCam : public ACameraActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMenuConfigCam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
