// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTriggerWidget.h"
#include "CommonTextBlock.h"
#include "UIProjectPlayerController.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Input/CommonUIInputTypes.h"
#include "MainMenuUI/RootWidget.h"

void UInGameTriggerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IA_Confirm)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Confirm, false,
			FSimpleDelegate::CreateUObject(this, &UInGameTriggerWidget::OpenLevel)));
	}

	if (IA_Cancel)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Cancel, false,
			FSimpleDelegate::CreateUObject(this, &UInGameTriggerWidget::OnCloseMenu)));
	}
}

void UInGameTriggerWidget::OpenLevel()
{
	
}

void UInGameTriggerWidget::OnCloseMenu()
{
	DeactivateWidget();
}

void UInGameTriggerWidget::SetText(const FName& MapName)
{
	if (PlaceText)
	{
		PlaceText->SetText(FText::FromName(MapName));
	}
}
