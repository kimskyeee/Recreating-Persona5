// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawnChar.generated.h"

UCLASS()
class UIPROJECT_API AMainMenuPawnChar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuPawnChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(visibleAnywhere)
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(visibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(visibleAnywhere)
	class UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category="Anim")
	UAnimMontage* LandToStandMontage;

	UPROPERTY(EditAnywhere, Category="Fall")
	float Gravity = 980.f;
	UPROPERTY(EditAnywhere, Category="Fall")
	float StartZ = 450.f;
	UPROPERTY(EditAnywhere, Category="Fall")
	float GroundZ = 250.f; // 레벨 기준

	bool bFalling = true;
	bool bLanded = false;
	float VerticalVelocity = 0.f;

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HandleLanding();
	void SwitchLanding();

	UFUNCTION(BlueprintCallable)
	void OnEnteredLandLoop();

	FTimerHandle SitTimerHandle;
};
