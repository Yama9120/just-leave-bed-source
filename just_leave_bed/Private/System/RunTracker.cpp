// Fill out your copyright notice in the Description page of Project Settings.

#include "System/RunTracker.h"

void URunTracker::AddGameTime(float DeltaTime)
{
	// 経過時間を加算する
	RunTime += DeltaTime;
}

void URunTracker::AddRetry()
{
	// リトライ回数を加算する
	RetryCount++;
}

void URunTracker::ResetRun()
{
	// 経過時間とリトライ回数をリセットする
	RunTime = 0.0f;
	RetryCount = 0;
}

float URunTracker::GetRunTime() const
{
	return RunTime;
}

int32 URunTracker::GetRetryCount() const
{
	return RetryCount;
}
