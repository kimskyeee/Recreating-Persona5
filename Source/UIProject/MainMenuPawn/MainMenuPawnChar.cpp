// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawnChar.h"

#include "UIProjectPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTag/UIGameplayTagInfo.h"
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
	SpringArmComp->SetRelativeLocationAndRotation(FVector(-40,50,55), FRotator(-20.0f, -100.0f, -2.0f));
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeLocation(FVector(0,0,-150));

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

	if (UAnimInstance* Anim = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
	{
		float Len = Anim->Montage_Play(LandToStandMontage, 1.f);
		Anim->Montage_JumpToSection(FName("StartPose"), LandToStandMontage);
		Anim->Montage_Pause(LandToStandMontage);
		Anim->Montage_SetNextSection(FName("StartPose"), FName("LandLoop"), LandToStandMontage);
	}
}

// Called every frame
void AMainMenuPawnChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bFalling || bLanded) return;

	VerticalVelocity -= Gravity * DeltaTime;
	FVector Loc = GetActorLocation();
	Loc.Z += VerticalVelocity * DeltaTime;

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
	
	if (UAnimInstance* Anim = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
	{
		Anim->Montage_Resume(LandToStandMontage);
	}
	
	bFalling = false;
	bLanded  = true;
}

void AMainMenuPawnChar::OnEnteredLandLoop()
{
	if (UAnimInstance* Anim = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
	{
		Anim->Montage_Pause(LandToStandMontage); // 앉은 자세에서 멈춤
		GetWorldTimerManager().SetTimer(SitTimerHandle, this,
		&AMainMenuPawnChar::SwitchLanding, 0.5f, false);
	}
}

void AMainMenuPawnChar::SwitchLanding()
{
	if (AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetController()))
	{
		ACameraActor* PressCam = Cast<APressScreenCam>(UGameplayStatics::GetActorOfClass(GetWorld(), APressScreenCam::StaticClass()));
		PC->SwitchCameraWithShake(PressCam, 0.2f);
		PC->EnsureRootCreated(TAG_UI_Screen_PressAnyKey);
	}

	UAnimInstance* Anim = MeshComp->GetAnimInstance();
	if (!Anim || !LandToStandMontage) return;
	
	Anim->Montage_Resume(LandToStandMontage);
	Anim->Montage_JumpToSection(FName("Stand"), LandToStandMontage);
}

