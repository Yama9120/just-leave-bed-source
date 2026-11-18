// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/Gimmick.h"
#include "System/RetrySubsystem.h"

void AGimmick::SetGimmickEnabled(bool bNewEnabled)
{
	bEnabled = bNewEnabled;
	UE_LOG(LogTemp, Log, TEXT("Gimmick: %s %s."), *GetName(), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void AGimmick::BeginPlay()
{
	Super::BeginPlay();

	// ギミックの有効状態を、エディタで選んだ最初の状態にする
	bEnabled = bStartEnabled;

	URetrySubsystem* RetrySubsystem = GetWorld()->GetSubsystem<URetrySubsystem>();
	if (!RetrySubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gimmick: RetrySubsystem not found. %s will not stop on retry."), *GetName());
		return;
	}

	// チェックポイント到達時とミスからの復帰時に、ギミックを待機状態に戻す
	RetrySubsystem->OnCheckpointReached.AddDynamic(this, &AGimmick::HandleSectionEnd);
	RetrySubsystem->OnRetryStarted.AddDynamic(this, &AGimmick::HandleSectionEnd);
}

void AGimmick::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	URetrySubsystem* RetrySubsystem = GetWorld()->GetSubsystem<URetrySubsystem>();
	if (RetrySubsystem)
	{
		// サブレベルのアンロードで消えるときに、通知のバインドを解除する
		RetrySubsystem->OnCheckpointReached.RemoveDynamic(this, &AGimmick::HandleSectionEnd);
		RetrySubsystem->OnRetryStarted.RemoveDynamic(this, &AGimmick::HandleSectionEnd);
	}

	Super::EndPlay(EndPlayReason);
}

// 子クラスやBPで上書きして、ギミックの開始処理を書く
void AGimmick::Start_Implementation()
{
}

// 子クラスやBPで上書きして、待機状態に戻す後片付けを書く
void AGimmick::StopGimmick_Implementation()
{
}

void AGimmick::CompleteGimmick()
{
	// ミスで物が消えたときに完了と判定しないよう、ミス処理の最中は完了にしない
	URetrySubsystem* RetrySubsystem = GetWorld()->GetSubsystem<URetrySubsystem>();
	if (RetrySubsystem && RetrySubsystem->IsMissInProgress())
	{
		UE_LOG(LogTemp, Log, TEXT("Gimmick: %s completion ignored during miss."), *GetName());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Gimmick: %s completed."), *GetName());
	OnCompleted.Broadcast();
}

void AGimmick::OnPlayerEnter(APlayerCharacter* Player)
{
	if (!bEnabled)
	{
		UE_LOG(LogTemp, Log, TEXT("Gimmick: %s is disabled. Ignored player enter."), *GetName());
		return;
	}
	else if (bRunning)
	{
		return;
	}
	else
	{
		bRunning = true;
		Start();
	}
}

void AGimmick::HandleSectionEnd()
{
	// 区間が終わったら、他から切り替えられた状態も最初に戻す（まだ始まっていないギミックも戻すため、bRunning の判定より前に書く）
	bEnabled = bStartEnabled;

	if (!bRunning)
	{
		return;
	}
	else
	{
		// フラグは基底で戻す（BPでStopGimmickを上書きしても戻し忘れないように）
		bRunning = false;
		StopGimmick();
	}
}
