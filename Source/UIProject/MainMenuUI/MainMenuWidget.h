// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMenuCam : uint8 { Press, First, Second, Third, Fourth };

UCLASS()
class UIPROJECT_API UMainMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton1;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton2;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton3;
	UPROPERTY(meta = (BindWidget))
	class UCommonButtonBase* MainMenuButton4;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* IA_Quit;
	void OnQuitGame();
	
	virtual void NativeOnActivated() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UPROPERTY()
	class AUIProjectPlayerController* PC;
	UPROPERTY()
	float BlendTime = 0.2f;

	void RequestCam(EMenuCam Cam);
	
	void OnHoveredFirstButton();
	void OnHoveredSecondButton();
	void OnHoveredThirdButton();
	void OnHoveredFourthButton();
	
	void OnClickedFirstButton();
	void OnClickedSecondButton();
	void OnClickedThirdButton();
	void OnClickedFourthButton();

	void ActiveButton(bool bActive);
};
