// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayableBound.generated.h"

UCLASS()
class UIPROJECT_API APlayableBound : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayableBound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Bounds;

	UFUNCTION(BlueprintCallable)
	void GetWorldXYMinMax(FVector2D& OutMin, FVector2D& OutMax) const;
};
