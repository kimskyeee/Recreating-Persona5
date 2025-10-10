// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "BaseWidget.generated.h"

/**
 * 우선순위 판정
 * 인풋
 */
UCLASS()
class UIPROJECT_API UBaseWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	const FGameplayTag& GetScreenTag() const { return ScreenTag; }
	UFUNCTION(BlueprintCallable)
	const FGameplayTag& GetLayerTag() const { return LayerTag; }

	UFUNCTION(BlueprintCallable)
	bool IsBlockingInput() const { return bBlockGameInput; }
	UFUNCTION(BlueprintCallable, Category="Policy")
	void SetBlocking(bool bBlock) { bBlockGameInput = bBlock; }

protected:
	virtual void NativeOnActivated() override;
	
	UPROPERTY()
	TObjectPtr<class URootWidget> RootWidget;
	bool bBlockGameInput = true; // true일때 UI IMC
	
private:
	UPROPERTY(EditAnywhere)
	FGameplayTag ScreenTag;
	UPROPERTY(EditAnywhere)
	FGameplayTag LayerTag;
	
};
