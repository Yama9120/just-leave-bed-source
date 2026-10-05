// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Door.h"
#include "Curves/CurveFloat.h"
#include "TimerManager.h"

ADoor::ADoor()
{
	bEnableFloating = false;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorHinge = CreateDefaultSubobject<USceneComponent>(TEXT("DoorHingeMesh"));
	DoorPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorPanelMesh"));
	DoorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimelineComp"));

	// コンポーネントの階層を設定
	DoorFrame->SetupAttachment(RootComponent);
	DoorHinge->SetupAttachment(DoorFrame);
	DoorPanel->SetupAttachment(DoorHinge);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	UpdateFunctionFloat.BindDynamic(this, &ADoor::UpdateTimelineComp);

	if (DoorTimelineFloatCurve)
	{
		// Timelineコンポーネントにカーブを追加
		DoorTimelineComp->AddInterpFloat(DoorTimelineFloatCurve, UpdateFunctionFloat);
	}
}

void ADoor::OnPlayerOverlap(APlayerCharacter* Player)
{
	// ドアを開く
	DoorTimelineComp->Play();

	// ドアを閉じるタイマーを設定
	GetWorldTimerManager().SetTimer(DoorCloseTimerHandle, this, &ADoor::CloseDoor, DoorCloseDuration, false);
}

void ADoor::UpdateTimelineComp(float Output)
{
	// ドアの回転を更新
	FRotator DoorNewRotation = FRotator(0.0f, Output, 0.f);
	DoorHinge->SetRelativeRotation(DoorNewRotation);
}

void ADoor::CloseDoor()
{
	// ドアを閉じる
	DoorTimelineComp->Reverse();
}
