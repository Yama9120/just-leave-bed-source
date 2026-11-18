// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/Launcher.h"
#include "Gimmicks/LaunchPoint.h"

void ALauncher::BeginPlay()
{
	Super::BeginPlay();

	// 全 LaunchPoint の OnLaunchFinished に HandlePointFinished をつなぐ
	for (int32 i = 0; i < LaunchPoints.Num(); ++i)
	{
		if (LaunchPoints[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Launch point at index %d is null for launcher: %s"), i, *GetName());
			continue;
		}
		// 有効な LaunchPoint があれば、OnLaunchFinished に HandlePointFinished をバインドする
		LaunchPoints[i]->OnLaunchFinished.AddDynamic(this, &ALauncher::HandlePointFinished);
	}
}

void ALauncher::Start_Implementation()
{
	FinishedCount = 0;

	if (LaunchPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No launch points set for launcher: %s"), *GetName());
		return;
	}

	// LaunchPoints配列をループして、各LaunchPointのLaunch()を呼び出す
	for (int32 i = 0; i < LaunchPoints.Num(); ++i)
	{
		if (LaunchPoints[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Launch point at index %d is null for launcher: %s"), i, *GetName());
			continue;
		}

		// 有効なLaunchPointがあればLaunch()を呼び出して発射する
		LaunchPoints[i]->Launch();
	}
}

void ALauncher::StopGimmick_Implementation()
{
	FinishedCount = 0;

	// 配下の発射地点をすべて止める（nullの警告はStart側で出しているので、ここでは黙ってスキップする）
	for (int32 i = 0; i < LaunchPoints.Num(); ++i)
	{
		if (LaunchPoints[i] == nullptr)
		{
			continue;
		}

		// 有効なLaunchPointがあればStopLaunch()を呼び出して発射を停止する
		LaunchPoints[i]->StopLaunch();
	}
}

void ALauncher::HandlePointFinished()
{
	FinishedCount++;
	// 全ての発射地点が完了したら、ギミック完了を通知する
	if (FinishedCount >= LaunchPoints.Num())
	{
		CompleteGimmick();
	}
}
