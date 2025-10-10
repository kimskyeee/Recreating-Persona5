// Fill out your copyright notice in the Description page of Project Settings.


#include "MapWidget.h"

void UMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	bBlockGameInput = false;
}

TOptional<FUIInputConfig> UMapWidget::GetDesiredInputConfig() const
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
