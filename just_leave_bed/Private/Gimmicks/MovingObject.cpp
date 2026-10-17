// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/MovingObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gimmicks/ResultTags.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/ResultReceiver.h"
#include "Kismet/GameplayStatics.h"
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

	// OnProjectileStopが呼ばれたときにOnStop関数を呼ぶように設定
	Movement->OnProjectileStop.AddDynamic(this, &AMovingObject::OnStop);
}

void AMovingObject::OnPlayerEnter(APlayerCharacter* Player)
{
	Super::OnPlayerEnter(Player);

	// プレイヤーにあたっても、物体は消えないようにするため、通知だけ行う
	Notify(ResultTags::FailHit);
}

void AMovingObject::OnStop(const FHitResult& HitResult)
{
	Destroy();
}

void AMovingObject::Notify(FGameplayTag Tag)
{
	// すでに通知済みの場合は何もしない
	if (bNotified)
	{
		return;
	}

	// 受け取り側に関係なく、この物体は通知済みとして確定する
	bNotified = true;

	// プレイヤーキャラクターを取得
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		// 受け取り側のResultReceiverを取得
		UResultReceiver* Receiver = Player->GetResultReceiver();

		if (Receiver)
		{
			// ResultReceiverに通知する
			Receiver->Receive(Tag, GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MovingObject: ResultReceiver is not set on %s."), *GetNameSafe(Player));
		}
	}
}
