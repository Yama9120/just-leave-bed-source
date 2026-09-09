// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/StageTransitionTrigger.h"
#include "Characters/PlayerCharacter.h"
#include "System/StageStreamingSubsystem.h"

void AStageTransitionTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 重なったアクターがプレイヤーキャラクターかどうかを確認
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		UStageStreamingSubsystem* StageStreamingSubsystem = GetGameInstance()->GetSubsystem<UStageStreamingSubsystem>();
		if (!StageStreamingSubsystem)
		{
			return;
		}

		// 特定の位置（ロード/アンロード地点）に来た場合、bIsPreloadの値に応じてレベルをロードまたはアンロードする
		if (bIsPreload)
		{
			StageStreamingSubsystem->LoadStage(TargetStageName);
		}
		else
		{
			StageStreamingSubsystem->UnloadStage(TargetStageName);
		}
	}
}
