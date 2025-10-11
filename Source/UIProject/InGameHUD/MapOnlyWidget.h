// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI/BaseWidget.h"
#include "MapOnlyWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIPROJECT_API UMapOnlyWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	void OnCloseMenu();
	void OnZoomMap();

	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

	UPROPERTY(meta=(BindWidget))
	class UCommonTextBlock* MapText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayableBound> PlayableBound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Cancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Zoom;
	
private:
	// virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;

	float Zoom = 1.0f;
	
	// 월드맵
	FVector2D MapMin = FVector2D::ZeroVector;
	FVector2D MapMax = FVector2D::ZeroVector;

	// 지도 텍스처에서 지도가 있는 부분 (사용할 부분)
	FVector2D UVMin = FVector2D(0.3f, 0.4f);
	FVector2D UVMax = FVector2D(0.68f, 0.6f);

	// 지도 이미지 크기
	FVector2D MapSize = FVector2D(1300.f, 1300.f);

	void WorldToMapUV();
};
