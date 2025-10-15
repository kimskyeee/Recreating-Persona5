// Fill out your copyright notice in the Description page of Project Settings.


#include "TopMenuWidget.h"
#include "CommonButtonBase.h"
#include "Kismet/GameplayStatics.h"

void UTopMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTopMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (SystemButton)
	{
		SystemButton->OnClicked().AddUObject(this, &UTopMenuWidget::OnClickSystemButton);
	}

	FindPostProcessVolume();
	SetWeightBlend(1);
}

void UTopMenuWidget::NativeOnDeactivated()
{
	SetWeightBlend(0);
	Super::NativeOnDeactivated();
}

void UTopMenuWidget::OnClickSystemButton()
{
	
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
