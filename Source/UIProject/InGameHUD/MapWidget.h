// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "MapWidget.generated.h"

/**
 * 
 */

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
	class UImage* FootImage;

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

private:
	float UOffset = 0;
	float VOffset = 0;
	bool bStart = false;

	float StartU = 0.0f;
	float StartV = 0.0f;
	float Center = 0.5f;
};
