// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "InGameWidget.generated.h"
/**
 * 
 */
UCLASS()
class UIPROJECT_API UInGameWidget : public UBaseWidget
{
	GENERATED_BODY()
private:
	FDelegateHandle HudAnimHandle;
	
	virtual void NativeOnInitialized() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeAnim;

	void OnHudMenuAnim(bool bForward);
};
