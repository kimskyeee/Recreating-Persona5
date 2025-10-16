// Fill out your copyright notice in the Description page of Project Settings.


#include "QuitGameModal.h"

#include "CommonButtonBase.h"

void UQuitGameModal::NativeOnActivated()
{
	QuitYesButton->OnClicked().AddUObject(this, &UQuitGameModal::OnClickedYes);
	QuitNoButton->OnClicked().AddUObject(this, &UQuitGameModal::OnClickedNo);
	Super::NativeOnActivated();
}

void UQuitGameModal::NativeOnDeactivated()
{
	if (QuitYesButton)
	{
		QuitYesButton->OnClicked().RemoveAll(this);
	}
	if (QuitNoButton)
	{
		QuitNoButton->OnClicked().RemoveAll(this);
	}
	Super::NativeOnDeactivated();
}

void UQuitGameModal::OnClickedYes()
{
	// 게임 종료
	UE_LOG(LogTemp, Display, TEXT("게임종료"));
}

void UQuitGameModal::OnClickedNo()
{
	DeactivateWidget();
}
