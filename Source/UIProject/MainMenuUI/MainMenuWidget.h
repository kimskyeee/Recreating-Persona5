// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UMainMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton1;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton2;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton3;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton4;
};
