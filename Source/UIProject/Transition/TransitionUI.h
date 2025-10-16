// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "TransitionUI.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UTransitionUI : public UBaseWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
	void HandleFadeOutFinished();
	
protected:
	virtual void NativeOnActivated() override;
	
public:
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* OpenLevelAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* AfterLevelAnim;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Transition")
	FName TargetMapName;
};
