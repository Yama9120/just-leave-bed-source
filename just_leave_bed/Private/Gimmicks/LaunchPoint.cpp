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

		// 発射した物体が消えたときに通知を受け取るようにする
		SpawnedObject->OnDestroyed.AddDynamic(this, &ALaunchPoint::HandleObjectDestroyed);

		// 発射した物体の数をカウントする
		LaunchedCount++;
	}
}

void ALaunchPoint::Launch()
{
	// 発射中フラグを立てる
	LaunchedCount = 0;
	bLaunching = true;

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

	// 発射中フラグを下ろす
	bLaunching = false;

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

void ALaunchPoint::HandleObjectDestroyed(AActor* DestroyedActor)
{
	if (!bLaunching)
	{
		return;
	}

	// 発射した物体が消えたので、配列から削除する
	// OnDestroyed は消える途中で呼ばれ、まだ有効と判定されるため、先に配列から外す
	SpawnedObjects.Remove(Cast<AMovingObject>(DestroyedActor));

	CheckFinished();
}

void ALaunchPoint::CheckFinished()
{
	// 発射間隔がない時は一回のみの発射なので、発射予定の数を１にする
	int32 PlannedCount = 1;
	if (Interval > 0.0f)
	{
		PlannedCount = Count;
	}

	// 発射予定の数が0以下の場合は、打ち続けるため発射終了通知を送らない
	if (PlannedCount <= 0)
	{
		return;
	}

	// 発射した数が予定の数に達していない場合は、発射終了通知を送らない
	if (LaunchedCount < PlannedCount)
	{
		return;
	}

	// 発射した物体がまだ残っている場合は、発射終了通知を送らない
	for (const TWeakObjectPtr<AMovingObject>& Object : SpawnedObjects)
	{
		if (Object.IsValid())
		{
			return;
		}
	}

	// 二重に通知しないよう、発射中フラグを下ろす（この後に物が消えても HandleObjectDestroyed で無視される）
	bLaunching = false;
	UE_LOG(LogTemp, Log, TEXT("LaunchPoint %s finished launching."), *GetName());
	// 発射終了通知を送る
	OnLaunchFinished.Broadcast();
}
