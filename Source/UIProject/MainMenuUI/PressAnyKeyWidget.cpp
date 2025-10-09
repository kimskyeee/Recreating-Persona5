// Fill out your copyright notice in the Description page of Project Settings.


#include "PressAnyKeyWidget.h"
#include "MainMenuWidget.h"
#include "RootWidget.h"
#include "UIProjectPlayerController.h"
#include "CommonButtonBase.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/MenuNewGameCam.h"

void UPressAnyKeyWidget::GoMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("GoMainMenu"));
	UWorld* World = GetWorld();
	AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(World->GetFirstPlayerController());
	if (!PC) return;

	// 카메라 전환
	AMenuNewGameCam* MenuFirstCam = Cast<AMenuNewGameCam>(UGameplayStatics::GetActorOfClass(GetWorld(), AMenuNewGameCam::StaticClass()));
	PC->SwitchCameraWithShake(MenuFirstCam, 0.2f);

	// UI 스택에 push
	PC->RootWidget->PushByTag(TAG_UI_Screen_MainMenu);

	DeactivateWidget();
}
