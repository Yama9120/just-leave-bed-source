// Fill out your copyright notice in the Description page of Project Settings.

#include "System/StageStreamingSubsystem.h"
#include "Kismet/GameplayStatics.h"

// レベルを非同期でロードする
void UStageStreamingSubsystem::LoadStage(FName StageName)
{
	FLatentActionInfo LatentInfo;

	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnStageLoaded");
	LatentInfo.UUID = NextUUID++; // 複数レベルのロードが同時に行われる場合に備えて、UUIDをインクリメント（同一UUIDのLatentアクションは進行中の再呼び出しが無視される）
	LatentInfo.Linkage = 0;

	PendingLoadStage = StageName;

	// bShouldBlockOnLoadをfalseに設定して,ロード完了を待たずにゲームを動かし続ける(非同期)
	// LoadStreamLevelが終わったらExecutionFunctionの関数が呼ばれる（エンジン内部での実装）
	UGameplayStatics::LoadStreamLevel(this, StageName, true, false, LatentInfo);
}

// レベルを非同期でアンロードする
void UStageStreamingSubsystem::UnloadStage(FName StageName)
{
	FLatentActionInfo LatentInfo;

	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnStageUnloaded");
	LatentInfo.UUID = NextUUID++; // 複数レベルのアンロードが同時に行われる場合に備えて、UUIDをインクリメント（同一UUIDのLatentアクションは進行中の再呼び出しが無視される）
	LatentInfo.Linkage = 0;

	PendingUnloadStage = StageName;

	// bShouldBlockOnUnloadをfalseに設定して,アンロード完了を待たずにゲームを動かし続ける(非同期)
	// UnloadStreamLevelが終わったらExecutionFunctionの関数が呼ばれる（エンジン内部での実装）
	UGameplayStatics::UnloadStreamLevel(this, StageName, LatentInfo, false);
}

// レベルロード完了時に呼ばれる
void UStageStreamingSubsystem::OnStageLoaded()
{
	LoadedStages.Add(PendingLoadStage);
	UE_LOG(LogTemp, Log, TEXT("Stage loaded: %s"), *PendingLoadStage.ToString());
}

// レベルアンロード完了時に呼ばれる
void UStageStreamingSubsystem::OnStageUnloaded()
{
	LoadedStages.Remove(PendingUnloadStage);
	UE_LOG(LogTemp, Log, TEXT("Stage unloaded: %s"), *PendingUnloadStage.ToString());
}
