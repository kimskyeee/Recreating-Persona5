// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "TopMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UTopMenuWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ShakeAnim;

	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* SystemButton;

private:
	UPROPERTY()
	APostProcessVolume* PostProcessVolume;
	
	void FindPostProcessVolume();
	void SetWeightBlend(float Active);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	void OnClickSystemButton();
};
