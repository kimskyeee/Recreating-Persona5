// Fill out your copyright notice in the Description page of Project Settings.

#include "MapWidget.h"
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

	/*// Zoom 반영
	const float Z = FMath::Clamp(Zoom, 0.001f, 1.0f);
	const float CropU = FMath::Max(0.f, UVMax.X - UVMin.X);
	const float CropV = FMath::Max(0.f, UVMax.Y - UVMin.Y);
	
	const float MarginU = 0.5f * Z * CropU;
	const float MarginV = 0.5f * Z * CropV;
	
	// 허용 가능한 중심 범위(축별)
	const float UminAllowed = UVMin.X + MarginU;
	const float UmaxAllowed = UVMax.X - MarginU;
	const float VminAllowed = UVMin.Y + MarginV;
	const float VmaxAllowed = UVMax.Y - MarginV;
	
	// 실제 클램프
	U = FMath::Clamp(U, UminAllowed, UmaxAllowed);
	V = FMath::Clamp(V, VminAllowed, VmaxAllowed);*/
	
	// Zoom 반영
	const float Z = FMath::Clamp(Zoom, 0.001f, 1.0f);
	const float Margin = 0.5f * Z;
	U = FMath::Clamp(U, Margin, 1.f - Margin);
	V = FMath::Clamp(V, Margin, 1.f - Margin);

	MI_Map->SetScalarParameterValue("PlayerU", U);
	MI_Map->SetScalarParameterValue("PlayerV", V);
	MI_Map->SetScalarParameterValue("Zoom", Z);
}

void UMapWidget::UpdateMarkerRotation(APawn* MarkerPawn)
{
	const float Yaw = MarkerPawn->GetActorRotation().Yaw;
	PlayerImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	PlayerImage->SetRenderTransformAngle(Yaw);
}
