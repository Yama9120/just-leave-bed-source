// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/Gimmick.h"
#include "System/RetrySubsystem.h"

void AGimmick::BeginPlay()
{
	Super::BeginPlay();

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

void AGimmick::OnPlayerEnter(APlayerCharacter* Player)
{
	if (bRunning)
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
