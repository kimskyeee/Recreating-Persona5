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

	UMaterialInterface* MI_FootOrigin = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/UIMaterial/M_MapFoot.M_MapFoot"));
	MI_Foot = UMaterialInstanceDynamic::Create(MI_FootOrigin, this);
	MI_Foot->SetTextureParameterValue("MapTex", FootTexture);
	MI_Foot->SetScalarParameterValue("Zoom", Zoom);
	
	MapImage->SetBrushFromMaterial(MI_Map);
	FootImage->SetBrushFromMaterial(MI_Foot);

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
}

void UMapWidget::UpdateFromWorldLocation(const FVector& WorldLoc)
{
	const float Dx = MapMax.X - MapMin.X;
	const float Dy = MapMax.Y - MapMin.Y;

	// 월드 좌표 정규화(0~1)
	const float Uw = FMath::GetRangePct(MapMin.X, MapMax.X, WorldLoc.X);
	const float Vw = FMath::GetRangePct(MapMin.Y, MapMax.Y, WorldLoc.Y);
	
	// 실제 범위 계산
	float U = FMath::Lerp(UVMin.X, UVMax.X, Uw);
	float V = FMath::Lerp(UVMin.Y, UVMax.Y, Vw);
	
	// Zoom 반영
	const float Z = FMath::Clamp(Zoom, 0.001f, 1.0f);
	const float Margin = 0.5f * Z;
	U = FMath::Clamp(U, Margin, 1.f - Margin);
	V = FMath::Clamp(V, Margin, 1.f - Margin);
	
	MI_Map->SetScalarParameterValue("PlayerU", U);
	MI_Map->SetScalarParameterValue("PlayerV", V);
	MI_Map->SetScalarParameterValue("Zoom", Z);

	// 지도 이동량(반대 방향 오프셋)
	if (!bStart)
	{
		StartU = U; // U0
		StartV = V; // V0

		// 이후 매 프레임에 사용할 보정치(중앙 정렬을 만들기 위한 오프셋)
		// (U - UOffset) == Center 가 되도록 설정 → t0 순간 발자국이 화면 중앙에 찍힘
		UOffset = StartU - Center;
		VOffset = StartV - Center;

		UE_LOG(LogTemp, Warning, TEXT("[MiniMap] StartU=%f StartV=%f  UOffset=%f VOffset=%f"),
			   StartU, StartV, UOffset, VOffset);

		bStart = true;
	}

	MI_Foot->SetScalarParameterValue("PlayerU", U - UOffset);
	MI_Foot->SetScalarParameterValue("PlayerV", V - VOffset);
	MI_Foot->SetScalarParameterValue("Zoom", Z);
}

void UMapWidget::UpdateMarkerRotation(APawn* MarkerPawn)
{
	const float Yaw = MarkerPawn->GetActorRotation().Yaw;
	PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	PlayerImage->SetRenderTransformAngle(Yaw);
}