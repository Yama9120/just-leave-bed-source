// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/Launcher.h"
#include "Gimmicks/LaunchPoint.h"

void ALauncher::Start_Implementation()
{
	if (LaunchPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No launch points set for launcher: %s"), *GetName());
		return;
	}

	// LaunchPoints配列をループして、各LaunchPointのFire()を呼び出す
	for (int32 i = 0; i < LaunchPoints.Num(); ++i)
	{
		if (LaunchPoints[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Launch point at index %d is null for launcher: %s"), i, *GetName());
			continue;
		}

		// 有効なLaunchPointがあればFire()を呼び出して発射する
		LaunchPoints[i]->Fire();
	}
}
