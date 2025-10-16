// Fill out your copyright notice in the Description page of Project Settings.


#include "ABillboard.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AABillboard::AABillboard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	SetRootComponent(ScreenMesh);

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture2D"));
	Capture->SetupAttachment(RootComponent);
	Capture->bCaptureEveryFrame = true; // 움직이는걸 반영하기위해...
	Capture->bCaptureOnMovement = false;
	Capture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
}

// Called when the game starts or when spawned
void AABillboard::BeginPlay()
{
	Super::BeginPlay();

	CreateAndBindRenderTarget(); // 960x540 기본 생성
	SetupCaptureQuality(); // 성능 최적화
	SyncFromPlayerView(); // 초기 동기화

	for (int i=0;i<2;++i){
		RT[i] = NewObject<UTextureRenderTarget2D>(this);
		RT[i]->RenderTargetFormat = RTF_RGBA8;
		RT[i]->bForceLinearGamma = true;
		RT[i]->bAutoGenerateMips = false;
		RT[i]->InitAutoFormat(960,540);
		RT[i]->UpdateResourceImmediate(true);
	}

	Capture->TextureTarget = RT[WriteIndex]; // 캡처는 Write
	ScreenMID->SetTextureParameterValue(TEXT("ScreenTex"), RT[ReadIndex]); // 화면은 Read

	// 자기 자신은 숨김
	// Capture->HiddenActors.Add(this);

	// 레벨 내 모든 전광판 스크린을 숨김 목록에 추가
	/*TArray<AActor*> AllScreens;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), AllScreens);
	for (AActor* Item : AllScreens)
	{
		if (Item && Item != this)
		{
			Capture->HiddenActors.Add(Item);
		}
	}*/
}

// Called every frame
void AABillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SyncFromPlayerView();

	Swap(ReadIndex, WriteIndex);
	Capture->TextureTarget = RT[WriteIndex];
	ScreenMID->SetTextureParameterValue(TEXT("ScreenTex"), RT[ReadIndex]);
}

void AABillboard::CreateAndBindRenderTarget(int32 Width, int32 Height)
{
	RenderTarget = NewObject<UTextureRenderTarget2D>(this, TEXT("BillboardRT"));
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8; // LDR
	RenderTarget->bForceLinearGamma = true;
	RenderTarget->bAutoGenerateMips = false;
	RenderTarget->InitAutoFormat(Width, Height);
	RenderTarget->UpdateResourceImmediate(true);

	Capture->TextureTarget = RenderTarget;

	if (ScreenMaterialBase)
	{
		ScreenMID = UMaterialInstanceDynamic::Create(ScreenMaterialBase, this);
		ScreenMesh->SetMaterial(0, ScreenMID);
		ScreenMID->SetTextureParameterValue(TEXT("ScreenTex"), RenderTarget);
	}
}

void AABillboard::SetupCaptureQuality()
{
	// 큰 비용 항목끄기 (성능)
	auto& SF = Capture->ShowFlags;
	SF.AmbientOcclusion = 0;
	SF.ScreenSpaceReflections = 0;
	SF.VolumetricFog = 0;
	SF.Fog = 0;
	SF.Bloom = 0; 
	SF.MotionBlur = 0;
	SF.LumenReflections = 0;
	SF.LumenGlobalIllumination = 0;

	// 노출 고정(전광판 깜빡임 방지)
	Capture->PostProcessBlendWeight = 1.0f;
	Capture->PostProcessSettings.bOverride_AutoExposureMethod = 1;
	Capture->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	Capture->PostProcessSettings.bOverride_AutoExposureBias = 1;
	Capture->PostProcessSettings.AutoExposureBias = 0.0f;

	// 뷰거리 제한
	// Capture->MaxViewDistanceOverride = 20000.0f;
}

void AABillboard::SyncFromPlayerView()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	const FMinimalViewInfo& View = PC->PlayerCameraManager->GetCameraCacheView();
	Capture->SetWorldLocationAndRotation(View.Location, View.Rotation);
	Capture->FOVAngle = View.FOV;

	// 후처리 일치
	Capture->PostProcessSettings = View.PostProcessSettings;
	Capture->PostProcessBlendWeight = 1.0f;
}