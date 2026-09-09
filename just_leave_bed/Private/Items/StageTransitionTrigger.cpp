// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/StageTransitionTrigger.h"
#include "System/StageStreamingSubsystem.h"

void AStageTransitionTrigger::OnPlayerOverlap(APlayerCharacter* Player)
{
	// 特定の位置（ロード/アンロード地点）に来た場合の処理
	UStageStreamingSubsystem* StageStreamingSubsystem = GetGameInstance()->GetSubsystem<UStageStreamingSubsystem>();
	if (!StageStreamingSubsystem)
	{
		return;
	}

	// bIsPreloadの値に応じてレベルをロードまたはアンロードする
	if (bIsPreload)
	{
		StageStreamingSubsystem->LoadStage(TargetStageName);
	}
	else
	{
		StageStreamingSubsystem->UnloadStage(TargetStageName);
	}
}
