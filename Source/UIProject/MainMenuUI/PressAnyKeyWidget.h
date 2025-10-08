// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"

#include "PressAnyKeyWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UPressAnyKeyWidget : public UBaseWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
};
