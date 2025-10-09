// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawnChar.h"

#include "UIProjectPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuUI/UICam/PressScreenCam.h"


// Sets default values
AMainMenuPawnChar::AMainMenuPawnChar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComp->OnComponentHit.AddDynamic(this, &AMainMenuPawnChar::OnCapsuleHit);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 100.f;
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0,0,110), FRotator(0.0f, 240.0f, -20.0f));
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainMenuPawnChar::BeginPlay()
{
	Super::BeginPlay();

	FVector Loc = GetActorLocation();
	Loc.Z = StartZ;
	SetActorLocation(Loc);

	bFalling = true;
	bLanded = false;
	
	if (AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
		// SetViewTargetWithBlend(PressCam, 0.1f);
	}
}

// Called every frame
void AMainMenuPawnChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bFalling || bLanded) return;

	VerticalVelocity -= Gravity * GetWorld()->GetDeltaSeconds();
	FVector Loc = GetActorLocation();
	Loc.Z += VerticalVelocity * GetWorld()->GetDeltaSeconds();

	if (Loc.Z <= GroundZ)
	{
		Loc.Z = GroundZ;
		SetActorLocation(Loc);
		HandleLanding();
		return;
	}

	SetActorLocation(Loc, true);
}

// Called to bind functionality to input
void AMainMenuPawnChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainMenuPawnChar::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bLanded && bFalling && Hit.Normal.Z > 0.5f)
	{
		HandleLanding();
	}
}

void AMainMenuPawnChar::HandleLanding()
{
	if (bLanded) return;
	bLanded = true;
	bFalling = false;

	// 카메라 전환 + 셰이크
	if (AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetController()))
	{
		ACameraActor* PressCam = Cast<APressScreenCam>(UGameplayStatics::GetActorOfClass(GetWorld(), APressScreenCam::StaticClass()));
		PC->SwitchCameraWithShake(PressCam, 0.2f);
		PC->EnsureRootCreated();
	}
}
