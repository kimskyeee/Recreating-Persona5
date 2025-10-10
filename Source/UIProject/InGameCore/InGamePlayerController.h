// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIProjectPlayerController.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API AInGamePlayerController : public AUIProjectPlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void CameraSet() override;
	
public:
};
