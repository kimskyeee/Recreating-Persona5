// Fill out your copyright notice in the Description page of Project Settings.


#include "MapOnlyWidget.h"

#include "Components/Image.h"
#include "Groups/CommonButtonGroupBase.h"
#include "InGameCore/PlayableBound.h"
#include "CommonTextBlock.h"
#include "UIProjectPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"

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

void UMapOnlyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RecalcIfViewportChanged();
}

void UMapOnlyWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	RecalcIfViewportChanged();
	WorldToMapUV();
	Zoom = 0.8f;
	MapImage->SetRenderScale(FVector2D(Zoom, Zoom));
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

void UMapOnlyWidget::WorldToMapUV()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return;

	const FVector2D ViewportSize = CachedViewportSize;
	const float DpiScale = FMath::Max(CachedDpiScale, 0.0001f);
	
	FVector2D RedPos = RedMinUV * ViewportSize;
	FVector2D RedSize = (RedMaxUV - RedMinUV) * ViewportSize; 

	FVector2D WorldLoc = FVector2D(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
	float Uw = FMath::GetRangePct(MapMin.X, MapMax.X, WorldLoc.X);
	float Vw = FMath::GetRangePct(MapMin.Y, MapMax.Y, WorldLoc.Y);

	float X = RedPos.X + Uw * RedSize.X;
	float Y = RedPos.Y + Vw * RedSize.Y;

	const float Yaw = Pawn->GetActorRotation().Yaw;
	PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	PlayerImage->SetRenderTransformAngle(Yaw);
	
	const FVector2D MarkerPos = FVector2D(X, Y) / DpiScale;
	
	if (!PlayerImage) return;
	if (UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PlayerImage))
	{
		// 앵커를 좌상단 고정(절대 좌표 기반), 피벗은 마커 중앙이라면 (0.5, 0.5)
		CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 0.f, 0.f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f)); // 마커 피벗이 중앙일 때
		CanvasSlot->SetPosition(MarkerPos);
	}
}

void UMapOnlyWidget::RecalcIfViewportChanged()
{
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	const float DpiScale = UWidgetLayoutLibrary::GetViewportScale(this);

	const bool bSizeChanged = !ViewportSize.Equals(CachedViewportSize, KINDA_SMALL_NUMBER);
	const bool bDpiChanged  = !FMath::IsNearlyEqual(DpiScale, CachedDpiScale);

	if (!(bSizeChanged || bDpiChanged))
	{
		return; // 변화 없음
	}

	CachedViewportSize = ViewportSize;
	CachedDpiScale = DpiScale;

	WorldToMapUV();
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

	if (Zoom < 0.9f)
	{
		Zoom = 1.05f;
		PlayZoomIn(true);
		MapText->SetText(FText::FromString(TEXT("축소")));
	}
	else
	{
		Zoom = 0.8f;
		PlayZoomIn(false);
		MapText->SetText(FText::FromString(TEXT("확대")));
	}
}
