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
	AMovingObject* SpawnedObject = World->SpawnActor<AMovingObject>(ObjectClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
	if (SpawnedObject)
	{
		// 止めるときに消せるように、出した物体を覚えておく（寿命切れなどで自分から消えることもあるので弱参照）
		SpawnedObjects.Add(SpawnedObject);
	}
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

void ALaunchPoint::StopLaunch()
{
	// 遅れ・間隔の発射を止める
	GetWorldTimerManager().ClearTimer(Timer);

	// 出した物体を消す（Destroyでは失敗の通知は出ない。寿命切れはLifeSpanExpiredで拾っているため）
	for (const TWeakObjectPtr<AMovingObject>& Object : SpawnedObjects)
	{
		// すでに寿命で消えた物体は無効になっているので、残っているものだけ消す
		if (Object.IsValid())
		{
			Object->Destroy();
		}
	}

	// 配列を初期化する
	SpawnedObjects.Empty();
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