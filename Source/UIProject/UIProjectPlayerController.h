// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIProjectPlayerController.generated.h"

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

private:
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
	
	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleAnyBlockingUIActive(bool bAnyBlocking);

	void ApplyGameOnly();
	void ApplyGameAndUI();
	void ApplyUIOnly();

	void EnsureRootCreated();
	void BindRootDelegates();
	void UnbindRootDelegates();

public:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URootWidget> RootWidgetClass;
	UPROPERTY()
	TObjectPtr<URootWidget> RootWidget;
	
	UFUNCTION(BlueprintCallable, Category="CameraShake")
	void SwitchCameraWithShake(AActor* NewCamera, float BlendTime);

	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeClass;
};
