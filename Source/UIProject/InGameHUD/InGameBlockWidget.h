// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "InGameBlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UInGameBlockWidget : public UBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Confirm;
	
private:
	virtual void NativeOnInitialized() override;
	void OnCloseMenu();
};
