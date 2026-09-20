// Fill out your copyright notice in the Description page of Project Settings.

#include "AlarmTimingWidget.h"
#include "UI/Menu/MenuButton.h"
#include "Components/ProgressBar.h"

void UAlarmTimingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StopButton)
	{
		StopButton->OnClicked.AddUniqueDynamic(this, &UAlarmTimingWidget::OnStopButtonClicked);
	}
}

void UAlarmTimingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ゲージの速度に応じて位置を更新
	GaugeValue += GaugeDirection * GaugeSpeed * InDeltaTime;

	// ゲージが端に到達したら方向を反転
	if (GaugeValue >= 1.0f)
	{
		GaugeValue = 1.0f;
		GaugeDirection = -1.0f;
	}
	else if (GaugeValue <= 0.0f)
	{
		GaugeValue = 0.0f;
		GaugeDirection = 1.0f;
	}

	// ゲージの表示を更新
	if (TimingGauge)
	{
		TimingGauge->SetPercent(GaugeValue);
	}
}

void UAlarmTimingWidget::OnStopButtonClicked(UMenuButton* Button)
{
	TryStopAlarm();
}

void UAlarmTimingWidget::TryStopAlarm()
{
	UE_LOG(LogTemp, Log, TEXT("GaugeValue: %f"), GaugeValue);

	// 判定窓の範囲内かどうかをチェック
	if (GaugeValue >= JudgeWindowCenter - JudgeWindowHalfWidth && GaugeValue <= JudgeWindowCenter + JudgeWindowHalfWidth)
	{
		// 判定窓内で停止した場合、ミニゲーム成功
		FinishMinigame(true);
	}
	else
	{
		// 判定窓外で停止した場合、ミニゲーム失敗
		FinishMinigame(false);
	}
}
