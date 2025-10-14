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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnActivated() override;

	UPROPERTY(meta=(BindWidget))
	class UCommonTextBlock* MapText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* MapAnim;

	void PlayZoomIn(bool bForward);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayableBound> PlayableBound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Cancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Zoom;
	
private:
	// virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CanvasPanel;
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;

	float Zoom = 0.8f;

	FVector2D RedMinUV = FVector2D(0.35f, 0.34f);
	FVector2D RedMaxUV = FVector2D(0.64f, 0.65f);

	FVector2D CachedViewportSize = FVector2D::ZeroVector;
	float CachedDpiScale = 0.f;
	
	// 월드맵
	FVector2D MapMin = FVector2D::ZeroVector;
	FVector2D MapMax = FVector2D::ZeroVector;
	
	void WorldToMapUV();
	void RecalcIfViewportChanged();
};
