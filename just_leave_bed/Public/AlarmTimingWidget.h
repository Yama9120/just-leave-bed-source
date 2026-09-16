// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DreamGame/MinigameBase.h"
#include "AlarmTimingWidget.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UAlarmTimingWidget : public UMinigameBase
{
	GENERATED_BODY()

private:
	// ゲージの移動速度(毎秒の増加分)。1.0で片道1秒
	UPROPERTY(EditDefaultsOnly, Category = "AlarmTiming", meta = (ClampMin = "0.0"))
	float GaugeSpeed = 1.0f;
	// 判定窓の半幅（判定窓の幅は JudgeWindowHalfWidth * 2）
	UPROPERTY(EditDefaultsOnly, Category = "AlarmTiming", meta = (ClampMin = "0.0"))
	float JudgeWindowHalfWidth = 0.1f;
};
