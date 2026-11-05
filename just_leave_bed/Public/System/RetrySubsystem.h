// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "RetrySubsystem.generated.h"

class APlayerCharacter;
class APlayerController;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API URetrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleResult(FGameplayTag ResultTag);
	void RegisterCheckpoint(const FTransform& Transform);

private:
	void StartMissSequence();
	void BeginVanishSequence();
	void BeginAppearSequence();
	void EndMissSequence();
	void RespawnPlayer();

	APlayerCharacter* FindPlayerCharacter() const;
	APlayerController* FindPlayerController() const;

	bool bMissInProgress = false;
	FTransform RespawnTransform;

	float VanishDuration = 0.3f; // 消えてから現れるまでの時間（秒）
	float AppearDuration = 0.3f; // 現れてから操作を戻すまでの（秒）
	FTimerHandle MissTimerHandle;
};
