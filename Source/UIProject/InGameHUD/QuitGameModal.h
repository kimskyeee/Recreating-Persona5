// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "QuitGameModal.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UQuitGameModal : public UBaseWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* QuitYesButton;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* QuitNoButton;
	
private:
	void OnClickedYes();
	void OnClickedNo();
};
