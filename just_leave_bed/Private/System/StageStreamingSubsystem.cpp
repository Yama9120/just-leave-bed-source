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

	// bShould
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
