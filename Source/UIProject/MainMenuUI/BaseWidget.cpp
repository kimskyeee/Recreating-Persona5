// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"
#include "InputAction.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/DataTable.h"
#include "Engine/EngineBaseTypes.h"
#include "Input/CommonUIInputTypes.h"

void UBaseWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	bIsFocusable = true;
}
