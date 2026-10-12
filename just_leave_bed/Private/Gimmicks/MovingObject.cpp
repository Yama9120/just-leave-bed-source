// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/MovingObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMovingObject::AMovingObject()
{
	PrimaryActorTick.bCanEverTick = false;

	Box->SetCollisionProfileName(TEXT("MovingObject"));
	Object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Object->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Object->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	// メッシュは当たり判定のBoxの子なので、MovementのUpdatedComponentはBoxに設定する
	Movement->UpdatedComponent = Box;
	// 初期速度と最大速度を設定
	Movement->InitialSpeed = 1000.0f;
	Movement->MaxSpeed = 2000.0f;
	// バウンドしないように設定（当たったら終了として利用するため）
	Movement->bShouldBounce = false;
}

void AMovingObject::BeginPlay()
{
	Super::BeginPlay();

	// 指定された秒数後にオブジェクトを破棄する
	SetLifeSpan(Lifespan);
}
