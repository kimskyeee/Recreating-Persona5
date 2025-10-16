// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalTrigger.h"
#include "UIProjectPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameplayTag/UIGameplayTagInfo.h"
#include "InGameHUD/InGameTriggerWidget.h"
#include "MainMenuUI/RootWidget.h"


// Sets default values
APortalTrigger::APortalTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APortalTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APortalTrigger::OnMyBeginOverlap);
	}
}

void APortalTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAutoPromptOnOverlap || bPromptShown) return;
	if (!OtherActor || !OtherActor->ActorHasTag(TEXT("Player"))) return;
	AUIProjectPlayerController* PC = Cast<AUIProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;
	
	bPromptShown = true;
	if (bBlock)
	{
		PC->RootWidget->PushByTag(TAG_UI_Screen_InGameMenu_Block);
		bPromptShown = false;
	}
	else
	{
		if (UCommonActivatableWidget* NewScreen =
						PC->RootWidget->PushByTag(TAG_UI_Screen_InGameMenu_Trigger))
		if (UInGameTriggerWidget* TriggerUI = Cast<UInGameTriggerWidget>(NewScreen))
		{
			// 트리거가 가진 목적지 이름 전달
			TriggerUI->SetText(DestinationMapName);
		}
		bPromptShown = false;
	}
}

// Called every frame
void APortalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

