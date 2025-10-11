// Fill out your copyright notice in the Description page of Project Settings.


#include "MapOnlyWidget.h"

#include "Components/Image.h"
#include "Groups/CommonButtonGroupBase.h"
#include "InGameCore/PlayableBound.h"
#include "CommonTextBlock.h"
#include "UIProjectPlayerController.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;

void UMapOnlyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!PlayableBound)
	{
		PlayableBound = Cast<APlayableBound>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayableBound::StaticClass()));
		PlayableBound->GetWorldXYMinMax(MapMin, MapMax);
	}

	// C = 취소
	if (IA_Cancel)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Cancel, false,
			FSimpleDelegate::CreateUObject(this, &UMapOnlyWidget::OnCloseMenu)));
	}

	// Tab = 확대
	if (IA_Zoom)
	{
		RegisterUIActionBinding(FBindUIActionArgs(
			IA_Zoom, false,
			FSimpleDelegate::CreateUObject(this, &UMapOnlyWidget::OnZoomMap)));
	}
}

void UMapOnlyWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	WorldToMapUV();
	MapText->SetText(FText::FromString(TEXT("확대")));
}

/*TOptional<FUIInputConfig> UMapOnlyWidget::GetDesiredInputConfig() const
{
	FUIInputConfig Config(
		ECommonInputMode::Menu, 
		EMouseCaptureMode::CapturePermanently,
		EMouseLockMode::LockAlways
	);

	Config.bIgnoreLookInput = true;
	Config.bIgnoreMoveInput = true;

	return Config;
}*/

void UMapOnlyWidget::WorldToMapUV()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return;

	FVector2D Loc = FVector2D(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
	float Nx = (Loc.X - MapMin.X) / (MapMax.X - MapMin.X);
	float Ny = (Loc.Y - MapMin.Y) / (MapMax.Y - MapMin.Y);
	Nx = FMath::Clamp(Nx, 0.f, 1.f);
	Ny = FMath::Clamp(Ny, 0.f, 1.f);

	const float NyFlip = 1.f - Ny; // Y뒤집기
	
	// UV 사각형
	const float U = FMath::Lerp(UVMin.X, UVMax.X, Nx);
	const float V = FMath::Lerp(UVMin.Y, UVMax.Y, NyFlip);

	// 위젯 픽셀 좌표
	const float Xpix = U * MapSize.X;
	const float Ypix = V * MapSize.Y;

	const float Yaw = Pawn->GetActorRotation().Yaw;
	PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	// PlayerImage->SetRenderTransformAngle(Yaw);
	PlayerImage->SetRenderTranslation(FVector2D(Xpix, Ypix));
}

void UMapOnlyWidget::OnCloseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCloseMenu"));
	DeactivateWidget(); // Pop
	
	if (AUIProjectPlayerController* PC = GetOwningPlayer<AUIProjectPlayerController>())
	{
		PC->ApplyUIOnly();
	}
}

void UMapOnlyWidget::OnZoomMap()
{
	UE_LOG(LogTemp, Warning, TEXT("OnZoomMap"));
	Zoom = (Zoom < 1.5f) ? 1.8f : 1.0f;
	if (MapText)
	{
		if (Zoom > 1.5f)
		{
			MapText->SetText(FText::FromString(TEXT("축소")));
		}
		else
		{
			MapText->SetText(FText::FromString(TEXT("확대")));
		}
	}
	MapImage->SetRenderScale(FVector2D(Zoom, Zoom));
}
