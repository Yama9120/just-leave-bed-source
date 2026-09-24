// Fill out your copyright notice in the Description page of Project Settings.

#include "AlarmTimingWidget.h"
#include "UI/Menu/MenuButton.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UAlarmTimingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StopButton)
	{
		StopButton->OnClicked.AddUniqueDynamic(this, &UAlarmTimingWidget::OnStopButtonClicked);
	}
	if (ClearButton)
	{
		ClearButton->OnClicked.AddUniqueDynamic(this, &UAlarmTimingWidget::OnClearButtonClicked);
	}
}

void UAlarmTimingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ゲームがクリア済みの場合、ゲージの更新を停止
	if (bIsCleared)
	{
		return;
	}

	// スコアに応じたゲージの速度を取得
	const float Speed = GetCurrentSpeed();

	// ゲージの速度に応じて位置を更新
	GaugeValue += GaugeDirection * Speed * InDeltaTime;

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

void UAlarmTimingWidget::StartMinigame_Implementation()
{
	// ゲーム開始時にスコアとクリア状態をリセット
	Score = 0;
	bIsCleared = false;
	GaugeValue = 0.0f;
	GaugeDirection = 1.0f;

	// ゲーム開始時にClearButtonを非表示にする
	if (ClearButton)
	{
		ClearButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	UpdateJudgeWindowUI();
}

void UAlarmTimingWidget::OnStopButtonClicked(UMenuButton* Button)
{
	TryStopAlarm();
}

void UAlarmTimingWidget::OnClearButtonClicked(UMenuButton* Button)
{
	if (!bIsCleared)
	{
		return;
	}

	FinishMinigame(true);
}

void UAlarmTimingWidget::TryStopAlarm()
{
	// ゲームがクリア済みの場合、判定を行わない
	if (bIsCleared)
	{
		return;
	}

	// 判定窓の範囲内かどうかをチェック
	if (GaugeValue >= JudgeWindowCenter - JudgeWindowHalfWidth && GaugeValue <= JudgeWindowCenter + JudgeWindowHalfWidth)
	{
		// 判定窓内で停止した場合、スコアを加算
		Score++;
	}
	else
	{
		// 判定窓外で停止した場合、スコアを減算
		Score--;
	}
	// スコアをFailScoreとClearScoreの範囲内に制限
	Score = FMath::Clamp(Score, FailScore, ClearScore);

	// スコアに応じてゲームの結果を判定
	if (Score >= ClearScore)
	{
		bIsCleared = true;

		if (ClearButton)
		{
			ClearButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (Score <= FailScore)
	{
		FinishMinigame(false);
	}
}

void UAlarmTimingWidget::UpdateJudgeWindowUI()
{
	if (!JudgeWindowImage)
	{
		return;
	}

	// 判定窓の位置とサイズを更新
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(JudgeWindowImage->Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(JudgeWindowCenter - JudgeWindowHalfWidth, 0, JudgeWindowCenter + JudgeWindowHalfWidth, 1));
		CanvasSlot->SetOffsets(FMargin(0));
	}
}

float UAlarmTimingWidget::GetCurrentSpeed() const
{
	if (Score <= 0)
	{
		return GaugeSpeed;
	}
	else
	{
		return GaugeSpeed * FMath::Pow(GaugeSpeedRate, Score);
	}
}
