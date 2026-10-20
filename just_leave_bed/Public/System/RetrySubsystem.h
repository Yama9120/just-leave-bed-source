// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "RetrySubsystem.generated.h"

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

private:
	void StartMissSequence();
	bool bMissInProgress = false;
};
