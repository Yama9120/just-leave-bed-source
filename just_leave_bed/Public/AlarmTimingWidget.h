// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DreamGame/MinigameBase.h"
#include "AlarmTimingWidget.generated.h"

class UMenuButton;
class UProgressBar;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UAlarmTimingWidget : public UMinigameBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnStopButtonClicked(UMenuButton* Button);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> TimingGauge;
	// ゲージの現在位置（0.0～1.0）
	float GaugeValue = 0.0f;
	// ゲージの移動方向（1.0で右、-1.0で左）
	float GaugeDirection = 1.0f;
	// ゲージの移動速度(毎秒の増加分)。1.0で片道1秒
	UPROPERTY(EditDefaultsOnly, Category = "AlarmTiming", meta = (ClampMin = "0.0"))
	float GaugeSpeed = 1.0f;
	// 判定窓の半幅（判定窓の幅は JudgeWindowHalfWidth * 2）
	UPROPERTY(EditDefaultsOnly, Category = "AlarmTiming", meta = (ClampMin = "0.0"))
	float JudgeWindowHalfWidth = 0.1f;

	// アラーム停止ボタン
	// TODO: タイミング判定実装時に入力に変換予定
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StopButton;
};
