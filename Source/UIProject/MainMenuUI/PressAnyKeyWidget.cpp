// Fill out your copyright notice in the Description page of Project Settings.


#include "PressAnyKeyWidget.h"
#include "MainMenuWidget.h"
#include "RootWidget.h"
#include "UIProjectPlayerController.h"
#include "CommonButtonBase.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/MenuNewGameCam.h"
#include "Input/CommonUIInputTypes.h"

void UPressAnyKeyWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	UE_LOG(LogTemp, Warning, TEXT("PressAnyKeyWidget NativeOnActivated"));
}

FReply UPressAnyKeyWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressed: %s"), *PressedKey.GetDisplayName().ToString());
		GoMainMenu();
		return FReply::Handled(); // 입력을 소비
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

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
