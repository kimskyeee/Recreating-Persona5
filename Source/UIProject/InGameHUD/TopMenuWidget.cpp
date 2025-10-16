// Fill out your copyright notice in the Description page of Project Settings.


#include "TopMenuWidget.h"
#include "CommonButtonBase.h"
#include "UIProjectPlayerController.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuUI/RootWidget.h"

void UTopMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTopMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PlayAnimation(ShakeAnim);
	
	if (SystemButton)
	{
		SystemButton->OnClicked().AddUObject(this, &UTopMenuWidget::OnClickSystemButton);
	}

	FindPostProcessVolume();
	SetWeightBlend(1); //흑백
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UTopMenuWidget::NativeOnDeactivated()
{
	if (SystemButton)
	{
		SystemButton->OnClicked().RemoveAll(this);
	}
	SetWeightBlend(0);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	Super::NativeOnDeactivated();
}

void UTopMenuWidget::OnClickSystemButton()
{
	AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;

	// UI 스택에 push
	PC->RootWidget->PushByTag(TAG_UI_Screen_InGameMenu_QuitGame);
}

void UTopMenuWidget::FindPostProcessVolume()
{
	PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
    
	if (!PostProcessVolume) return;
}

void UTopMenuWidget::SetWeightBlend(float Active)
{
	if (!PostProcessVolume) return;

	if (PostProcessVolume->Settings.WeightedBlendables.Array.Num() > 2)
	{
		PostProcessVolume->Settings.WeightedBlendables.Array[2].Weight = Active;
		UE_LOG(LogTemp, Warning, TEXT("[SetWeightBlend] : %f"), Active);
	}
}
