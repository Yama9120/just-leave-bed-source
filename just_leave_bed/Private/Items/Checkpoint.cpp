// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Checkpoint.h"
#include "System/RetrySubsystem.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/PlayerCharacter.h"

ACheckpoint::ACheckpoint()
{
	// 復帰する位置と向きを示す矢印を作り、親が作ったBoxに付ける
	RespawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RespawnArrow"));
	RespawnArrow->SetupAttachment(Box);
}

void ACheckpoint::OnPlayerEnter(APlayerCharacter* Player)
{
	Super::OnPlayerEnter(Player);

	// 初めて到達したときだけ登録する（テレポートで範囲に入り直したときの再登録を防ぐ）
	if (!bIsReached)
	{
		URetrySubsystem* RetrySubsystem = GetWorld()->GetSubsystem<URetrySubsystem>();
		if (RetrySubsystem)
		{
			// 矢印のワールド座標での位置と向きを、復帰地点として登録する
			FTransform ArrowTransform = RespawnArrow->GetComponentTransform();
			RetrySubsystem->RegisterCheckpoint(ArrowTransform);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Checkpoint: RetrySubsystem not found. %s was not registered."), *GetName());
		}

		// チェックポイントに到達済みにする
		bIsReached = true;
	}
}

FTransform ACheckpoint::CalculateRespawnTransform(APlayerCharacter* Player)
{
	FTransform Result = RespawnArrow->GetComponentTransform();

	// 矢印の位置から真下へ線を飛ばす
	const FVector Start = Result.GetLocation();
	const FVector End = Start - FVector(0.f, 0.f, FloorTraceDistance);

	// 自分（チェックポイント）とプレイヤーには当たらないようにする
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(Player);

	FHitResult Hit;
	const bool bHitFloor = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	if (bHitFloor)
	{
		// 矢印の真下の床を調べ、キャラが床にちょうど立つ高さに補正
		const float HalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector Location = Start;
		Location.Z = Hit.ImpactPoint.Z + HalfHeight;
		Result.SetLocation(Location);
	}
	else if (!bHitFloor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint: No floor found below RespawnArrow on %s. Using arrow location as is."), *GetName());
	}

	return Result;
}
