// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

#include "UIProjectPlayerController.h"
#include "MainMenuUI/RootWidget.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bBlockGameInput = false;
	if (AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		HudAnimHandle = PC->RootWidget->OnHudMenuAnim.AddUObject(this, &UInGameWidget::OnHudMenuAnim);
	}
}

TOptional<FUIInputConfig> UInGameWidget::GetDesiredInputConfig() const
{
	FUIInputConfig Config(
		ECommonInputMode::Game, 
		EMouseCaptureMode::CapturePermanently,
		EMouseLockMode::LockAlways
	);

	Config.bIgnoreLookInput = false;
	Config.bIgnoreMoveInput = false;

	return Config;
}

void UInGameWidget::NativeDestruct()
{
	if (AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (HudAnimHandle.IsValid())
		{
			PC->RootWidget->OnHudMenuAnim.Remove(HudAnimHandle);
			HudAnimHandle.Reset();
		}
	}
	
	Super::NativeDestruct();
}

void UInGameWidget::OnHudMenuAnim(bool bForward)
{
	if (!ChangeAnim) return;
	if (bForward)
	{
		PlayAnimationForward(ChangeAnim);
	}
	else
	{
		PlayAnimationReverse(ChangeAnim);
	}
}
