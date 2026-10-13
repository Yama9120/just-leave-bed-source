// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/LaunchPoint.h"
#include "Gimmicks/MovingObject.h"
#include "Components/ArrowComponent.h"

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

	// スポーンするアクターの所有者を設定
	World->SpawnActor<AMovingObject>(ObjectClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
}
