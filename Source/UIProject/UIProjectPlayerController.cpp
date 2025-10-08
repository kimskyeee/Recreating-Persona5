// Copyright Epic Games, Inc. All Rights Reserved.


#include "UIProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "MainMenuUI/RootWidget.h"

AUIProjectPlayerController::AUIProjectPlayerController()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempGameIMC(TEXT("/Game/Input/IMC_Game.IMC_Game"));
	if (TempGameIMC.Succeeded())
	{
		IMC_Game = TempGameIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempUIIMC(TEXT("/Game/Input/IMC_UI.IMC_UI"));
	if (TempUIIMC.Succeeded())
	{
		IMC_UI = TempUIIMC.Object;
	}
}

void AUIProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnsureRootCreated(); // 루트 생성, 바인딩
	if (RootWidget)
	{
		RootWidget->PushByTag(TAG_UI_Screen_PressAnyKey); // 시작 UI
	}
}

void AUIProjectPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindRootDelegates();
	Super::EndPlay(EndPlayReason);
}

void AUIProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (IMC_Game)
		{
			Subsystem->AddMappingContext(IMC_Game, GamePriority);
		}
	}
}

void AUIProjectPlayerController::HandleAnyBlockingUIActive(bool bAnyBlocking)
{
	if (bAnyBlocking) ApplyGameAndUI();
	else ApplyGameOnly();
}

void AUIProjectPlayerController::ApplyGameOnly()
{
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsys->ClearAllMappings();
			if (IMC_Game) { Subsys->AddMappingContext(IMC_Game, GamePriority); }
		}
	}
}

void AUIProjectPlayerController::ApplyGameAndUI()
{
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsys->ClearAllMappings();
			if (IMC_UI) { Subsys->AddMappingContext(IMC_UI, UIPriority); }
			if (IMC_Game) { Subsys->AddMappingContext(IMC_Game, GamePriority); }
		}
	}
}

void AUIProjectPlayerController::EnsureRootCreated()
{
	if (!RootWidget)
	{
		RootWidget = CreateWidget<URootWidget>(this, RootWidgetClass.Get());
		if (RootWidget)
		{
			RootWidget->AddToViewport(0); // 한번만
			BindRootDelegates();

			RootWidget->PushByTag(TAG_UI_Screen_PressAnyKey);
		}
	}
	else if (RootWidget && !RootWidget->IsInViewport())
	{
		// Viewport가 끊겼다면 재부착
		RootWidget->AddToViewport(0);
	}
}

void AUIProjectPlayerController::BindRootDelegates()
{
	if (!RootWidget) return;
	RootWidget->OnUIStateChanged.AddDynamic(this, &AUIProjectPlayerController::HandleAnyBlockingUIActive);
}

void AUIProjectPlayerController::UnbindRootDelegates()
{
	if (!RootWidget) return;
	RootWidget->OnUIStateChanged.RemoveDynamic(this, &AUIProjectPlayerController::HandleAnyBlockingUIActive);
}
