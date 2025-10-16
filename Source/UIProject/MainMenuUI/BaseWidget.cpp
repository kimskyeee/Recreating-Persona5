// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"
#include "InputAction.h"
#include "RootWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/DataTable.h"
#include "Engine/EngineBaseTypes.h"
#include "Input/CommonUIInputTypes.h"

void UBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// C = 취소
	if (IA_Escape)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Escape, false,
			FSimpleDelegate::CreateUObject(this, &UBaseWidget::OnToggleMenu)));
	}

	InputMappingPriority = 200;

	if (!RootWidget)
	{
		// 위젯 트리 상위에서 RootWidget 찾기
		RootWidget = GetTypedOuter<URootWidget>();
	}
}

void UBaseWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	SetIsFocusable(true);
	SetFocus();
}

void UBaseWidget::OnToggleMenu()
{
	if (!RootWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UBaseWidget::OnToggleMenu() RootWidget null!!!!!"));
		return;
	}
		
	RootWidget->OnHandleEscape();
}
