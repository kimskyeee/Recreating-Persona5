// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "CommonButtonBase.h"
#include "RootWidget.h"
#include "UIProjectPlayerController.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "UICam/MenuConfigCam.h"
#include "UICam/MenuLoadGameCam.h"
#include "UICam/MenuNewGameCam.h"
#include "UICam/MenuQcam.h"
#include "UICam/PressScreenCam.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MainMenuButton1->SetFocus();

	if (MainMenuButton1)
	{
		MainMenuButton1->OnHovered().AddUObject(this, &UMainMenuWidget::OnHoveredFirstButton);
		MainMenuButton1->OnClicked().AddUObject(this, &UMainMenuWidget::OnClickedFirstButton);
	}
	if (MainMenuButton2)
	{
		MainMenuButton2->OnHovered().AddUObject(this, &UMainMenuWidget::OnHoveredSecondButton);
		MainMenuButton2->OnClicked().AddUObject(this, &UMainMenuWidget::OnClickedSecondButton);
	}
	if (MainMenuButton3)
	{
		MainMenuButton3->OnHovered().AddUObject(this, &UMainMenuWidget::OnHoveredThirdButton);
		MainMenuButton3->OnClicked().AddUObject(this, &UMainMenuWidget::OnClickedThirdButton);
	}
	if (MainMenuButton4)
	{
		MainMenuButton4->OnHovered().AddUObject(this, &UMainMenuWidget::OnHoveredFourthButton);
		MainMenuButton4->OnClicked().AddUObject(this, &UMainMenuWidget::OnClickedFourthButton);
	}
}

void UMainMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PC = Cast<AUIProjectPlayerController>(GetWorld()->GetFirstPlayerController());
}

TOptional<FUIInputConfig> UMainMenuWidget::GetDesiredInputConfig() const
{
	FUIInputConfig Config(
		ECommonInputMode::Menu, 
		EMouseCaptureMode::NoCapture,
		EMouseLockMode::DoNotLock
	);

	Config.bIgnoreLookInput = true;
	Config.bIgnoreMoveInput = true;

	return Config;
}

void UMainMenuWidget::RequestCam(EMenuCam Cam)
{
	if (PC)
	{
		switch (Cam)
		{
		case EMenuCam::Press:
			PC->SwitchCameraWithShake(PC->PressCam, BlendTime);
			break;
		case EMenuCam::First:
			PC->SwitchCameraWithShake(PC->MenuFirstCam, BlendTime);
			break;
		case EMenuCam::Second:
			PC->SwitchCameraWithShake(PC->MenuSecondCam, BlendTime);
			break;
		case EMenuCam::Third:
			PC->SwitchCameraWithShake(PC->MenuThirdCam, BlendTime);
			break;
		case EMenuCam::Fourth:
			PC->SwitchCameraWithShake(PC->MenuFourthCam, BlendTime);
			break;
		}
	}
}

void UMainMenuWidget::OnHoveredFirstButton()
{
	RequestCam(EMenuCam::First);
}

void UMainMenuWidget::OnHoveredSecondButton()
{
	RequestCam(EMenuCam::Second);
}

void UMainMenuWidget::OnHoveredThirdButton()
{
	RequestCam(EMenuCam::Third);
}

void UMainMenuWidget::OnHoveredFourthButton()
{
	RequestCam(EMenuCam::Fourth);
}

void UMainMenuWidget::OnClickedFirstButton()
{
	RequestCam(EMenuCam::First);
	
	if (!PC) return;
	PC->RootWidget->PushByTag(TAG_UI_Screen_InGameMenu_Transition);
}

void UMainMenuWidget::OnClickedSecondButton()
{
	RequestCam(EMenuCam::Second);
}

void UMainMenuWidget::OnClickedThirdButton()
{
	RequestCam(EMenuCam::Third);
}

void UMainMenuWidget::OnClickedFourthButton()
{
	RequestCam(EMenuCam::Fourth);
}
