// Fill out your copyright notice in the Description page of Project Settings.

#include "System/RetrySubsystem.h"
#include "Gimmicks/ResultTags.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void URetrySubsystem::HandleResult(FGameplayTag ResultTag)
{
	bool bIsFail = ResultTag.MatchesTag(ResultTags::Fail);

	if (bIsFail && bMissInProgress)
	{
		UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Ignored %s (miss sequence already in progress)."), *ResultTag.ToString());
	}
	else if (bIsFail && !bMissInProgress)
	{
		StartMissSequence();
	}
}

void URetrySubsystem::RegisterCheckpoint(const FTransform& Transform)
{
	RespawnTransform = Transform;

	UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Checkpoint registered at %s."), *Transform.GetLocation().ToString());
}

void URetrySubsystem::StartMissSequence()
{
	bMissInProgress = true;
	UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Miss sequence started."));

	// 仮：すぐに復帰させてミス処理を終える（消える・現れる演出の追加時に置き換える）
	RespawnPlayer();
	bMissInProgress = false;
}

void URetrySubsystem::RespawnPlayer()
{
	// プレイヤーキャラクターを取得
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrySubsystem: PlayerCharacter not found."));
		return;
	}

	// しゃがんでいる場合は立ち上がる
	if (Player->GetCharacterMovement()->IsCrouching())
	{
		Player->UnCrouch();
	}

	// チェックポイントの位置と向きへテレポートさせる
	Player->SetActorLocationAndRotation(RespawnTransform.GetLocation(), RespawnTransform.GetRotation(), false, nullptr, ETeleportType::TeleportPhysics);

	// テレポート前の速度が残らないように止める
	Player->GetCharacterMovement()->StopMovementImmediately();

	// カメラの向きもチェックポイントの向きに合わせる（キャラの向きとは別に管理されているため）
	AController* PlayerController = Player->GetController();
	if (PlayerController)
	{
		PlayerController->SetControlRotation(RespawnTransform.Rotator());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrySubsystem: Controller not found. Control rotation not updated."));
	}

	// テレポートの移動量を段差として補間しないように、自前の補間をリセットする
	Player->ResetSmoothingState();
}
