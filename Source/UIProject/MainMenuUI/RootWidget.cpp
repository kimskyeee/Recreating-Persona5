// Fill out your copyright notice in the Description page of Project Settings.

#include "RootWidget.h"

#include "BaseWidget.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Widgets/CommonActivatableWidgetContainer.h" 

void URootWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (MainStack)
	{
		MainChangedHandle = MainStack->OnDisplayedWidgetChanged().AddUObject(this, &URootWidget::HandleMainChanged);
	}
	if (OverlayStack)
	{
		OverlayChangedHandle = OverlayStack->OnDisplayedWidgetChanged().AddUObject(this, &URootWidget::HandleOverlayChanged);
	}
	if (ModalStack)
	{
		ModalChangedHandle = ModalStack->OnDisplayedWidgetChanged().AddUObject(this, &URootWidget::HandleModalChanged);
	}
	
	HandleMainChanged(nullptr);
	HandleOverlayChanged(nullptr);
	HandleModalChanged(nullptr);
}

void URootWidget::NativeDestruct()
{
	if (MainStack && MainChangedHandle.IsValid())
	{
		MainStack->OnDisplayedWidgetChanged().Remove(MainChangedHandle);
		MainChangedHandle.Reset();
	}
	if (OverlayStack && OverlayChangedHandle.IsValid())
	{
		OverlayStack->OnDisplayedWidgetChanged().Remove(OverlayChangedHandle);
		OverlayChangedHandle.Reset();
	}
	if (ModalStack && ModalChangedHandle.IsValid())
	{
		ModalStack->OnDisplayedWidgetChanged().Remove(ModalChangedHandle);
		ModalChangedHandle.Reset();
	}
	Super::NativeDestruct();
}

UCommonActivatableWidget* URootWidget::PushByTag(const FGameplayTag ScreenTag)
{
	if (!ScreenTag.IsValid()) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("URootWidget::PushByTag"));

	// 태그로 클래스 찾기
	TSubclassOf<UBaseWidget> ScreenClass = ScreenMap.FindRef(ScreenTag);
	if (!ScreenClass) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("ScreenClass is not NULL"));

	UBaseWidget* Base = ScreenClass.GetDefaultObject();
	FGameplayTag Layer = Base ? Base->GetLayerTag() : TAG_UI_Layer_Overlay;
	UCommonActivatableWidgetStack* Target = ChooseStackByLayer(Layer);

	// 스택에 추가후 포인터 반환
	return Target ? Target->AddWidget(*ScreenClass) : nullptr;
}

void URootWidget::HandleMainChanged(class UCommonActivatableWidget* NewTop)
{
	bMainBlocking = IsBlockingWidget(NewTop, false);
	RecalcAndBroadcast();
}

void URootWidget::HandleOverlayChanged(class UCommonActivatableWidget* NewTop)
{
	bOverlayBlocking = IsBlockingWidget(NewTop, true);
	RecalcAndBroadcast();
}

void URootWidget::HandleModalChanged(class UCommonActivatableWidget* NewTop)
{
	bModalBlocking = IsBlockingWidget(NewTop, true);
	RecalcAndBroadcast();
}

void URootWidget::HandleHUDChanged(class UCommonActivatableWidget* NewTop)
{
	bHUDBlocking = IsBlockingWidget(NewTop, false);
	RecalcAndBroadcast();
}

UCommonActivatableWidgetStack* URootWidget::ChooseStackByLayer(const FGameplayTag& LayerTag)
{
	UE_LOG(LogTemp, Warning, TEXT("ChooseStackByLayer"));
	if (LayerTag == TAG_UI_Layer_Main) return MainStack;
	if (LayerTag == TAG_UI_Layer_Overlay) return OverlayStack;
	if (LayerTag == TAG_UI_Layer_Modal) return ModalStack;
	if (LayerTag == TAG_UI_Layer_HUD) return HUDStack;
	return nullptr;
}

// 게임 입력 차단 여부
bool URootWidget::IsBlockingWidget(const UCommonActivatableWidget* Widget, bool bDefaultBlocking)
{
	if (!Widget) return false;
	const UBaseWidget* Screen = Cast<UBaseWidget>(Widget);
	return Screen ? Screen->IsBlockingInput() : bDefaultBlocking;
}

void URootWidget::RecalcAndBroadcast()
{
	const bool bAnyBlocking = (bModalBlocking || bOverlayBlocking || bMainBlocking);
	OnUIStateChanged.Broadcast(bAnyBlocking);
}
