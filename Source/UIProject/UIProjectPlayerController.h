// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIProjectPlayerController.generated.h"

class UInputAction;
struct FGameplayTag;
class URootWidget;
class UInputMappingContext;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AUIProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AUIProjectPlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void CameraSet();

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Game;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_UI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Combat;

	UPROPERTY(EditAnywhere, Category="Input")
	int32 GamePriority = 0;
	UPROPERTY(EditAnywhere, Category="Input")
	int32 UIPriority = 10;
	
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MapAction;
	
	UFUNCTION()
	void HandleAnyBlockingUIActive(bool bAnyBlocking);

	void BindRootDelegates();
	void UnbindRootDelegates();

	void OnToggleMenu();

public:
	void ApplyGameOnly();
	// void ApplyGameAndUI();
	void ApplyUIOnly();

	void OnCloseMenu();
	
public:
	void EnsureRootCreated(const FGameplayTag& InitialTag);
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URootWidget> RootWidgetClass;
	UPROPERTY()
	TObjectPtr<URootWidget> RootWidget;
	
	UFUNCTION(BlueprintCallable, Category="CameraShake")
	void SwitchCameraWithShake(AActor* NewCamera, float BlendTime);

	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeClass;

	UPROPERTY(editanywhere, BlueprintReadWrite)
	class APressScreenCam* PressCam;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class AMenuNewGameCam* MenuFirstCam;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class AMenuLoadGameCam* MenuSecondCam;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class AMenuQcam* MenuThirdCam;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class AMenuConfigCam* MenuFourthCam;
};
