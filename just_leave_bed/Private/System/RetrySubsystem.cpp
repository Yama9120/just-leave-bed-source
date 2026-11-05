// Fill out your copyright notice in the Description page of Project Settings.

#include "System/RetrySubsystem.h"
#include "Gimmicks/ResultTags.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

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
	// プレイヤーがいなければミス処理を始めない（フラグを立てる前に確認する）
	APlayerCharacter* Player = FindPlayerCharacter();
	if (!Player)
	{
		return;
	}

	// ミス演出中であることを示すフラグを立てる
	bMissInProgress = true;
	UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Miss sequence started."));

	// 入力はPawnだけ止める（コントローラー側のポーズ操作は効いたままにするため）
	APlayerController* PlayerController = FindPlayerController();
	if (PlayerController)
	{
		Player->DisableInput(PlayerController);
	}

	// プレイヤーキャラクターの消える演出を開始
	BeginVanishSequence();
}

void URetrySubsystem::BeginVanishSequence()
{
	APlayerCharacter* Player = FindPlayerCharacter();
	if (!Player)
	{
		return;
	}

	// プレイヤーキャラクターの消える演出を開始
	Player->PlayVanishEffect();

	// チェックポイントにテレポートさせる
	RespawnPlayer();

	// VanishDuration後に現れる段階へ進む（消えている間にカメララグでカメラが移動する）
	GetWorld()->GetTimerManager().SetTimer(MissTimerHandle, this, &URetrySubsystem::BeginAppearSequence, VanishDuration, false);
}

void URetrySubsystem::BeginAppearSequence()
{
	APlayerCharacter* Player = FindPlayerCharacter();
	if (!Player)
	{
		return;
	}

	// プレイヤーキャラクターの現れる演出を開始
	Player->PlayAppearEffect();

	// AppearDuration後にミス演出を終了する
	GetWorld()->GetTimerManager().SetTimer(MissTimerHandle, this, &URetrySubsystem::EndMissSequence, AppearDuration, false);
}

void URetrySubsystem::EndMissSequence()
{
	APlayerCharacter* Player = FindPlayerCharacter();
	APlayerController* PlayerController = FindPlayerController();

	if (Player && PlayerController)
	{
		// プレイヤーキャラクターの操作を戻す
		Player->EnableInput(PlayerController);
	}

	// プレイヤーがいなくてもフラグは必ず下ろす（以降のミスを受け付けられるように）
	bMissInProgress = false;
	UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Miss sequence ended."));
}

void URetrySubsystem::RespawnPlayer()
{
	// プレイヤーキャラクターを取得
	APlayerCharacter* Player = FindPlayerCharacter();
	if (!Player)
	{
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
	APlayerController* PlayerController = FindPlayerController();
	if (PlayerController)
	{
		PlayerController->SetControlRotation(RespawnTransform.Rotator());
	}

	// テレポートの移動量を段差として補間しないように、自前の補間をリセットする
	Player->ResetSmoothingState();
}

APlayerCharacter* URetrySubsystem::FindPlayerCharacter() const
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrySubsystem: PlayerCharacter not found."));
	}

	return Player;
}

APlayerController* URetrySubsystem::FindPlayerController() const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrySubsystem: PlayerController not found."));
	}

	return PlayerController;
}
