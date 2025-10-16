// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "InGameTriggerWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UInGameTriggerWidget : public UBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* PlaceText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Confirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Cancel;
	
	void SetText(const FName& MapName);
	
private:
	virtual void NativeOnInitialized() override;

	void OpenLevel();
	void OnCloseMenu();
};
