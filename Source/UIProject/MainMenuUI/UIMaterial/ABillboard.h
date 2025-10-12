// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABillboard.generated.h"

UCLASS()
class UIPROJECT_API AABillboard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABillboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, Category="Billboard")
	UStaticMeshComponent* ScreenMesh;
	UPROPERTY(VisibleAnywhere, Category="Billboard")
	USceneCaptureComponent2D* Capture;
	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;
	UPROPERTY(EditAnywhere, Category="Billboard")
	UMaterialInterface* ScreenMaterialBase;
	UPROPERTY()
	UMaterialInstanceDynamic* ScreenMID;

	void CreateAndBindRenderTarget(int32 Width = 960, int32 Height = 540);
	void SetupCaptureQuality();
	void SyncFromPlayerPOV(); // 현재 ViewTarget 복사용
};
