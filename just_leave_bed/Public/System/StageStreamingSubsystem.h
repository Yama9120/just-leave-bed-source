// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageStreamingSubsystem.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UStageStreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadStage(FName StageName);
	UFUNCTION(BlueprintCallable)
	void UnloadStage(FName StageName);
	UFUNCTION()
	void OnStageLoaded();
	UFUNCTION()
	void OnStageUnloaded();

private:
	// 同一UUIDは進行中の再呼び出しが無視されるので、呼び出しごとに一意な値を振る
	int32 NextUUID = 0;

	// ロード済みのステージ名を保持する
	TSet<FName> LoadedStages;
	FName PendingLoadStage;
	FName PendingUnloadStage;
};
