// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "RootWidget.generated.h"

class UBaseWidget;
class UCommonActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChanged, bool, bAnyBlockingActive);

UCLASS()
class UIPROJECT_API URootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FOnUIStateChanged OnUIStateChanged;

	// 태그로 푸시
	UFUNCTION(BlueprintCallable, Category="Stack")
	UCommonActivatableWidget* PushByTag(const FGameplayTag ScreenTag);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonActivatableWidgetStack> MainStack;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetStack> OverlayStack;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetStack> HUDStack; // 인게임에 계속 떠있는 UI
	
	UPROPERTY(EditDefaultsOnly, Category="Screens")
	TMap<FGameplayTag, TSubclassOf<UBaseWidget>> ScreenMap;
	
	// 스택 상태 추적
	FDelegateHandle MainChangedHandle;
	FDelegateHandle OverlayChangedHandle;
	FDelegateHandle ModalChangedHandle;
	FDelegateHandle HUDChangeHandle;

	bool bMainBlocking = false;
	bool bOverlayBlocking = false;
	bool bModalBlocking = false;
	bool bHUDBlocking = false;

	void HandleMainChanged(class UCommonActivatableWidget* NewTop);
	void HandleOverlayChanged(class UCommonActivatableWidget* NewTop);
	void HandleModalChanged(class UCommonActivatableWidget* NewTop);
	void HandleHUDChanged(class UCommonActivatableWidget* NewTop);

	UCommonActivatableWidgetStack* ChooseStackByLayer(const FGameplayTag& LayerTag);
	bool IsBlockingWidget(const UCommonActivatableWidget* Widget, bool bDefaultBlocking);
	void RecalcAndBroadcast();
};
