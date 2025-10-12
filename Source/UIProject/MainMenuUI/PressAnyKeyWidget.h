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
private:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void GoMainMenu();

	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* PressAnyButton;
};
