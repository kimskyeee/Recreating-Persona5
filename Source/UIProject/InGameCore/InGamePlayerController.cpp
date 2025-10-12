// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "MainMenuUI/RootWidget.h"


void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	EnsureRootCreated(TAG_UI_Screen_InGameHUD);
	if (RootWidget)
	{
		RootWidget->PushByTag(TAG_UI_Screen_InGameHUD); 
	}
	ApplyGameOnly();
}

void AInGamePlayerController::CameraSet()
{
	// 아무것도 하지 않음
}
