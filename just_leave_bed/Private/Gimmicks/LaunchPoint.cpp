// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/LaunchPoint.h"
#include "Gimmicks/MovingObject.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"

ALaunchPoint::ALaunchPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	SetRootComponent(Muzzle);
}

void ALaunchPoint::Fire()
{
	UWorld* const World = GetWorld();

	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in ALaunchPoint::Fire()"));
		return;
	}
	if (ObjectClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectClass is null in ALaunchPoint::Fire() for LaunchPoint: %s"), *GetName());
		return;
	}

	FActorSpawnParameters ActorSpawnParams;

	// 重なっていてもスポーンするように設定
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// スポーンするアクターを生成する
	World->SpawnActor<AMovingObject>(ObjectClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
}

void ALaunchPoint::Launch()
{
	if (Interval <= 0.0f && Delay <= 0.0f)
	{
		// 間隔も遅延もない場合は即座に発射
		Fire();
	}
	else if (Interval <= 0.0f && Delay > 0.0f)
	{
		// 遅延のみがある場合は、遅延後に一度だけ発射
		GetWorldTimerManager().SetTimer(Timer, this, &ALaunchPoint::Fire, Delay, false);
	}
	else if (Interval > 0.0f)
	{
		// 間隔がある場合は、遅延後に指定回数発射する（遅延はあってもなくても、Delay秒だけ遅延する）
		FireCount = 0;
		GetWorldTimerManager().SetTimer(Timer, this, &ALaunchPoint::OnTimer, Interval, true, Delay);
	}
}

void ALaunchPoint::OnTimer()
{
	Fire();
	FireCount++;

	if (Count > 0 && FireCount >= Count)
	{
		GetWorldTimerManager().ClearTimer(Timer);
	}
}