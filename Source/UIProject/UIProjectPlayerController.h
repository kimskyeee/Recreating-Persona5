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
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URootWidget> RootWidgetClass;
	UPROPERTY(Transient)
	TObjectPtr<URootWidget> RootWidget;
	
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Game;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_UI;

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

	void EnsureRootCreated();
	void BindRootDelegates();
	void UnbindRootDelegates();
};
