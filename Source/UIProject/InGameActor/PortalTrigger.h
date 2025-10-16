// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalTrigger.generated.h"

UCLASS()
class UIPROJECT_API APortalTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalTrigger();

protected:
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	bool bBlock = false;

	UPROPERTY(EditAnywhere, Category="Travel")
	FName DestinationMapName;
	
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp = nullptr;
	UPROPERTY(EditAnywhere, Category="Travel")
	bool bAutoPromptOnOverlap = true;

	bool bPromptShown = false;
};
