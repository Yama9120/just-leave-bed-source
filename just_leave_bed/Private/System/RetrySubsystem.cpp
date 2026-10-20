// Fill out your copyright notice in the Description page of Project Settings.

#include "System/RetrySubsystem.h"
#include "Gimmicks/ResultTags.h"

void URetrySubsystem::HandleResult(FGameplayTag ResultTag)
{
	bool bIsFail = ResultTag.MatchesTag(ResultTags::Fail);

	if (bIsFail && bMissInProgress)
	{
		UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Ignored %s (miss sequence already in progress)."), *ResultTag.ToString());
	}
	else if (bIsFail && !bMissInProgress)
	{
		StartMissSequence();
	}
}

void URetrySubsystem::StartMissSequence()
{
	bMissInProgress = true;

	UE_LOG(LogTemp, Log, TEXT("RetrySubsystem: Miss sequence started."));
}
