// Fill out your copyright notice in the Description page of Project Settings.


#include "MapOnlyWidget.h"

#include "Components/Image.h"
#include "Groups/CommonButtonGroupBase.h"
#include "InGameCore/PlayableBound.h"
#include "CommonTextBlock.h"
#include "UIProjectPlayerController.h"
#include "Components/CanvasPanelSlot.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"

class UCanvasPanelSlot;
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
	Zoom = 1.0f;
	MapImage->SetRenderScale(FVector2D(1.0f, 1.0f));
	MapText->SetText(FText::FromString(TEXT("확대")));
}

void UMapOnlyWidget::PlayZoomIn(bool bForward)
{
	if (bForward)
	{
		PlayAnimation(MapAnim);
	}
	else
	{
		PlayAnimationReverse(MapAnim);
	}
}

FPaintRect UMapOnlyWidget::GetBaseRect()
{
	FPaintRect Rect;
	if (!MapImage) return Rect;
	
	const FVector2D Alloc = MapImage->GetCachedGeometry().GetLocalSize();
	Rect.Width = Alloc.X; Rect.Height = Alloc.Y;
	Rect.Left = 0.f; Rect.Top = 0.f;
	return Rect;
}

FPaintRect UMapOnlyWidget::ApplyUVCrop(const FPaintRect& R, const FVector2D& InUVMin, const FVector2D& InUVMax)
{
	FPaintRect Out;
	const FVector2D UVSize = InUVMax - InUVMin;
	Out.Left = R.Left + R.Width  * InUVMin.X;
	Out.Top = R.Top  + R.Height * InUVMin.Y;
	Out.Width = R.Width  * UVSize.X;
	Out.Height = R.Height * UVSize.Y;
	return Out;
}

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

	const FPaintRect R = GetBaseRect();
	const FPaintRect RCrop = ApplyUVCrop(R, UVMin, UVMax);

	// 위젯 픽셀 좌표
	const float Xpix = RCrop.Left + Nx * RCrop.Width;
	const float Ypix = RCrop.Top  + Ny * RCrop.Height;
	
	const float Yaw = Pawn->GetActorRotation().Yaw;
	// PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	PlayerImage->SetRenderTransformAngle(Yaw);
	PlayerImage->SetRenderTranslation(FVector2D(Xpix, Ypix));

	/*if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PlayerImage->Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(0,0,0,0));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetPosition(FVector2D(Xpix, Ypix));
	}*/
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

	if (Zoom < 1.5f)
	{
		Zoom = 1.8f;
		PlayZoomIn(true);
		MapText->SetText(FText::FromString(TEXT("축소")));
	}
	else
	{
		Zoom = 1.0f;
		PlayZoomIn(false);
		MapText->SetText(FText::FromString(TEXT("확대")));
	}
}
