// Fill out your copyright notice in the Description page of Project Settings.

#include "MapWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InGameCore/PlayableBound.h"
#include "Kismet/GameplayStatics.h"

void UMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMaterialInterface* MI_Origin = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/UIMaterial/M_MapMask.M_MapMask"));
	MI_Map = UMaterialInstanceDynamic::Create(MI_Origin, this);
	MI_Map->SetTextureParameterValue("MapTex", MapTexture);
	MI_Map->SetScalarParameterValue("Zoom", Zoom);
	MapImage->SetBrushFromMaterial(MI_Map);

	FootImages = {
		FootImage0, FootImage1, FootImage2, FootImage3,
		FootImage4, FootImage5, FootImage6, FootImage7
	};

	InitPool();

	if (!PlayableBound)
	{
		PlayableBound = Cast<APlayableBound>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayableBound::StaticClass()));
		PlayableBound->GetWorldXYMinMax(MapMin, MapMax);
	}
}

void UMapWidget::NativeTick(const FGeometry& Geo, float DeltaSeconds)
{
	Super::NativeTick(Geo, DeltaSeconds);
	
	if (!MI_Map) return;
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return;

	UpdateFromWorldLocation(Pawn->GetActorLocation());
	UpdateMarkerRotation(Pawn);
	
	const float Speed = Pawn->GetVelocity().Size2D();
	if (Speed > MoveSpeedThreshold)
	{
		AccumDist += static_cast<double>(Speed) * static_cast<double>(DeltaSeconds);

		while (AccumDist >= StepDistanceWorld)
		{
			AccumDist -= StepDistanceWorld;
			SpawnFoot();
		}
	}

	UpdateFootprints();
}

void UMapWidget::UpdateFromWorldLocation(const FVector& WorldLoc)
{
	const float Dx = MapMax.X - MapMin.X;
	const float Dy = MapMax.Y - MapMin.Y;

	// 월드 좌표 정규화(0~1)
	const float Uw = FMath::GetRangePct(MapMin.X, MapMax.X, WorldLoc.X);
	const float Vw = FMath::GetRangePct(MapMin.Y, MapMax.Y, WorldLoc.Y);
	
	// 실제 범위 계산
	float Ur = FMath::Lerp(UVMin.X, UVMax.X, Uw);
	float Vr = FMath::Lerp(UVMin.Y, UVMax.Y, Vw);
	
	// Zoom 반영
	const float Z = FMath::Clamp(Zoom, 0.001f, 1.0f);
	const float Margin = 0.5f * Z;
	float Umap = FMath::Clamp(Ur, Margin, 1.f - Margin);
	float Vmap = FMath::Clamp(Vr, Margin, 1.f - Margin);
	
	MI_Map->SetScalarParameterValue("PlayerU", Umap);
	MI_Map->SetScalarParameterValue("PlayerV", Vmap);
	MI_Map->SetScalarParameterValue("Zoom", Z);

	UOffset = Umap - 0.5f;
	VOffset = Vmap - 0.5f;

	LastU = Ur;
	LastV = Vr;
	LastZ = Z;
}

void UMapWidget::UpdateMarkerRotation(APawn* MarkerPawn)
{
	const float Yaw = MarkerPawn->GetActorRotation().Yaw;
	PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	PlayerImage->SetRenderTransformAngle(Yaw);
}

void UMapWidget::InitPool()
{
	UMaterialInterface* MI_FootOrigin = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/UIMaterial/M_MapFoot.M_MapFoot"));

	FootPool.Reserve(FootImages.Num());
	for (int32 i = 0; i < FootImages.Num(); i++)
	{
		FFootItem FootSlot;
		FootSlot.MID = UMaterialInstanceDynamic::Create(MI_FootOrigin, this);
		FootSlot.MID->SetTextureParameterValue("MapTex", FootTexture);
		FootSlot.MID->SetScalarParameterValue("Zoom", Zoom);
		FootSlot.MID->SetScalarParameterValue("Opacity", 0.0f);

		if (FootImages[i])
		{
			FootImages[i]->SetBrushFromMaterial(FootSlot.MID);
		}

		FootPool.Add(FootSlot);
	}
}

void UMapWidget::SpawnFoot()
{
    APawn* Pawn = GetOwningPlayerPawn();
    if (!Pawn) return;
	
    FVector Dir = Pawn->GetVelocity().GetSafeNormal2D();
    if (Dir.IsNearlyZero())
    {
        Dir = Pawn->GetActorForwardVector(); // 멈췄을 때 정면
    }

	FVector Right = FVector::CrossProduct(FVector::UpVector, Dir).GetSafeNormal();
	float SideSign = (NextFootIndex % 2 == 0) ? -1.f : +1.f; // 짝수=왼, 홀수=오른
	
	FVector SpawnWorld = Pawn->GetActorLocation()
					   - Dir * BackOffset
					   + Right * SideSign * SideOffset;

    // 월드 -> 맵 UV 변환
    const float Uw = FMath::GetRangePct(MapMin.X, MapMax.X, SpawnWorld.X);
    const float Vw = FMath::GetRangePct(MapMin.Y, MapMax.Y, SpawnWorld.Y);

    const float Ur = FMath::Lerp(UVMin.X, UVMax.X, Uw);
    const float Vr = FMath::Lerp(UVMin.Y, UVMax.Y, Vw);

    FFootItem& FootSlot = FootPool[NextFootIndex];
    NextFootIndex = (NextFootIndex + 1) % FootPool.Num();

    if (!FootSlot.MID) return;

    FootSlot.FootU = Ur;
    FootSlot.FootV = Vr;

    FootSlot.SpawnTime = GetWorld()->GetTimeSeconds();
    FootSlot.bActive = true;
    FootSlot.LifeTime = 3.0f;

    const float U = 2.f * Center - (FootSlot.FootU - UOffset);
    const float V = 2.f * Center - (FootSlot.FootV - VOffset);
	
	const float YawDeg = FMath::UnwindDegrees(Pawn->GetActorRotation().Yaw);
	float FootYaw = FMath::Frac(1.f - (YawDeg / 360.f));
	FootYaw = FMath::Fmod(FootYaw + 1.0f, 1.0f);

    FootSlot.MID->SetScalarParameterValue("PlayerU", U);
    FootSlot.MID->SetScalarParameterValue("PlayerV", V);
    FootSlot.MID->SetScalarParameterValue("Zoom", LastZ);
    FootSlot.MID->SetScalarParameterValue("Opacity", 1.0f);
    FootSlot.MID->SetScalarParameterValue("FootYaw", FootYaw);
}

void UMapWidget::UpdateFootprints()
{
	const float Now = GetWorld()->GetTimeSeconds();

	for (FFootItem& S : FootPool)
	{
		if (!S.bActive || !S.MID) continue;

		// 현재 오프셋/줌으로 재주입 → 화면 고정 유지
		const float AnchorU = 2.f * Center - (S.FootU - UOffset);
		const float AnchorV = 2.f * Center - (S.FootV - VOffset);
		S.MID->SetScalarParameterValue(TEXT("PlayerU"), AnchorU);
		S.MID->SetScalarParameterValue(TEXT("PlayerV"), AnchorV);
		S.MID->SetScalarParameterValue(TEXT("Zoom"), LastZ);

		// 페이드 아웃
		const float Age = Now - S.SpawnTime;
		if (Age >= S.LifeTime)
		{
			S.bActive = false;
			S.MID->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
			continue;
		}
		const float Alpha = FMath::Clamp(1.f - Age / S.LifeTime, 0.f, 1.f);
		S.MID->SetScalarParameterValue(TEXT("Opacity"), Alpha);
	}
}
