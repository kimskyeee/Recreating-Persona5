// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameBlockWidget.h"
#include "Input/CommonUIInputTypes.h"

void UInGameBlockWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// C = 취소
	if (IA_Confirm)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Confirm, false,
			FSimpleDelegate::CreateUObject(this, &UInGameBlockWidget::OnCloseMenu)));
	}
}

void UInGameBlockWidget::OnCloseMenu()
{
	DeactivateWidget();
}
