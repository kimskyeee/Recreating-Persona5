// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bBlockGameInput = false;
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
