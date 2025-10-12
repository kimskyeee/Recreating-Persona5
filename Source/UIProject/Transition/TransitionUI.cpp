// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionUI.h"
#include "Kismet/GameplayStatics.h"

void UTransitionUI::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	// 애니메이션이 없으면 즉시 전환
	if (OpenLevelAnim)
	{
		// 애니메이션 종료 콜백 바인딩 후 재생
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UTransitionUI::HandleFadeOutFinished);
		BindToAnimationFinished(OpenLevelAnim, EndDelegate);
		PlayAnimationForward(OpenLevelAnim);
	}
	else
	{
		HandleFadeOutFinished();
	}
}

void UTransitionUI::HandleFadeOutFinished()
{
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, TargetMapName);
	}
}
