// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RunTracker.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API URunTracker : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddGameTime(float DeltaTime);
	void AddRetry();
	void ResetRun();
	float GetRunTime() const;
	int32 GetRetryCount() const;

private:
	float RunTime = 0.0f;
	int32 RetryCount = 0;
};
