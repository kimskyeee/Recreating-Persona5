// Copyright Epic Games, Inc. All Rights Reserved.


#include "UIProjectPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuUI/RootWidget.h"

#include "MainMenuUI/UICam/MenuConfigCam.h"
#include "MainMenuUI/UICam/MenuLoadGameCam.h"
#include "MainMenuUI/UICam/MenuNewGameCam.h"
#include "MainMenuUI/UICam/MenuQcam.h"
#include "MainMenuUI/UICam/PressScreenCam.h"

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
	CameraSet();
	// EnsureRootCreated(); // 루트 생성, 바인딩 -> 착지 시 해줌 비긴플레이 X
}

void AUIProjectPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindRootDelegates();
	Super::EndPlay(EndPlayReason);
}

void AUIProjectPlayerController::CameraSet()
{
	PressCam = Cast<APressScreenCam>(UGameplayStatics::GetActorOfClass(GetWorld(), APressScreenCam::StaticClass()));
	MenuFirstCam = Cast<AMenuNewGameCam>(UGameplayStatics::GetActorOfClass(GetWorld(), AMenuNewGameCam::StaticClass()));
	MenuSecondCam = Cast<AMenuLoadGameCam>(UGameplayStatics::GetActorOfClass(GetWorld(), AMenuLoadGameCam::StaticClass()));
	MenuThirdCam = Cast<AMenuQcam>(UGameplayStatics::GetActorOfClass(GetWorld(), AMenuQcam::StaticClass()));
	MenuFourthCam = Cast<AMenuConfigCam>(UGameplayStatics::GetActorOfClass(GetWorld(), AMenuConfigCam::StaticClass()));
}

void AUIProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (!IMC_UI) { UE_LOG(LogTemp, Error, TEXT("IMC_UI is NULL")); }
		if (IMC_UI)
		{
			Subsystem->AddMappingContext(IMC_UI, UIPriority);
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
			{
				EIC->BindAction(MapAction, ETriggerEvent::Started, this, &AUIProjectPlayerController::OnToggleMenu);
			}
		}
	}
}

void AUIProjectPlayerController::HandleAnyBlockingUIActive(bool bAnyBlocking)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleAnyBlockingUIActive"));
	if (bAnyBlocking) ApplyUIOnly();
	else ApplyGameOnly();
}

void AUIProjectPlayerController::ApplyGameOnly()
{
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsys->ClearAllMappings();
			if (IMC_Game) 
			{ 
				Subsys->AddMappingContext(IMC_Game, GamePriority); 
			}
			FInputModeGameOnly Mode;
			SetInputMode(Mode);
			SetShowMouseCursor(false);
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;
		}
	}
}

/*void AUIProjectPlayerController::ApplyGameAndUI()
{
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsys->ClearAllMappings();
			if (IMC_UI) 
			{ 
				Subsys->AddMappingContext(IMC_UI, UIPriority);
			}
			if (IMC_Game) 
			{ 
				Subsys->AddMappingContext(IMC_Game, GamePriority); 
			}
		}
	}
}*/

void AUIProjectPlayerController::ApplyUIOnly()
{
	UE_LOG(LogTemp, Warning, TEXT("ApplyUIOnly"));
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Subsys->ClearAllMappings();
			if (IMC_UI)
			{
				Subsys->AddMappingContext(IMC_UI, UIPriority);
				UE_LOG(LogTemp, Warning, TEXT("IMC_UI is not NULL"));
			}
			FInputModeUIOnly Mode;
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(Mode);
			SetShowMouseCursor(true);
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
		}
	}
}

void AUIProjectPlayerController::EnsureRootCreated(const FGameplayTag& InitialTag)
{
	if (!RootWidget)
	{
		RootWidget = CreateWidget<URootWidget>(this, RootWidgetClass.Get());
		if (RootWidget)
		{
			RootWidget->AddToViewport(0); // 한번만
			BindRootDelegates();

			RootWidget->PushByTag(InitialTag);
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

void AUIProjectPlayerController::OnToggleMenu()
{
	if (RootWidget)
	{
		RootWidget->PushByTag(TAG_UI_Screen_InGameMenu);
	}
}

void AUIProjectPlayerController::SwitchCameraWithShake(AActor* NewCamera, float BlendTime)
{
	if (!NewCamera) return;
	if (GetViewTarget() == NewCamera) return;
	
	SetViewTargetWithBlend(NewCamera, BlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f);
	if (ShakeClass && PlayerCameraManager)
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, [this]()
			{
				if (PlayerCameraManager && ShakeClass)
				{
					PlayerCameraManager->StartCameraShake(ShakeClass);
				}
			},
			FMath::Max(0.f, BlendTime), false);
	}
}
