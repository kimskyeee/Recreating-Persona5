// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "MapWidget.generated.h"

/**
 * 
 */
struct FFootItem
{
	UMaterialInstanceDynamic* MID = nullptr;
	bool  bActive = false;
	float SpawnTime = 0.f;
	float LifeTime  = 2.f; // 4초 후 소멸
	float FootU  = 0.f;
	float FootV = 0.f;
	float SpawnYaw = 0.f;
};

UCLASS()
class UIPROJECT_API UMapWidget : public UCommonUserWidget
{
	GENERATED_BODY()

private:
	FVector2D MapMin = FVector2D::ZeroVector;
	FVector2D MapMax = FVector2D::ZeroVector;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	void UpdateFromWorldLocation(const FVector& WorldLoc);
	void UpdateMarkerRotation(APawn* MarkerPawn);

	// 미니맵 텍스처 기준
	UPROPERTY(EditAnywhere)
	FVector2D UVMin = FVector2D(0.3f, 0.4f);
	UPROPERTY(EditAnywhere)
	FVector2D UVMax = FVector2D(0.68f, 0.6f);
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerImage;

	UPROPERTY(meta = (BindWidget))
	UImage* FootImage0;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage1;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage2;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage3;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage4;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage5;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage6;
	UPROPERTY(meta = (BindWidget))
	UImage* FootImage7;
	
	TArray<UImage*> FootImages;

	UPROPERTY()
	class UMaterialInstanceDynamic* MI_Map = nullptr;
	UPROPERTY(editanywhere)
	float Zoom = 0.2f;
	UPROPERTY(editanywhere)
	UTexture2D* MapTexture = nullptr;

	UPROPERTY()
	class UMaterialInstanceDynamic* MI_Foot = nullptr;
	UPROPERTY(editanywhere)
	UTexture2D* FootTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayableBound> PlayableBound;

	UPROPERTY(EditAnywhere)
	float BackOffset = 80.f;
	UPROPERTY(EditAnywhere)
	float SideOffset = 50.0f; // 좌우 폭

private:
	static constexpr int32 FootPoolSize = 8;
	TArray<FFootItem> FootPool;
	int32 NextFootIndex = 0;

	double StepDistanceWorld = 250.0;
	double MoveSpeedThreshold = 5.0;
	double AccumDist = 0.0;
	
	bool bStart = false;
	float UOffset = 0;
	float VOffset = 0;
	float StartU = 0.0f;
	float StartV = 0.0f;
	float Center = 0.5f;
	
	float LastU = 0.f;
	float LastV = 0.f;
	float LastZ = 0.2f;
	
	void InitPool();
	void SpawnFoot();
	void UpdateFootprints();  
};
