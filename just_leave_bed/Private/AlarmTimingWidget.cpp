// Fill out your copyright notice in the Description page of Project Settings.

#include "AlarmTimingWidget.h"
#include "UI/Menu/MenuButton.h"

void UAlarmTimingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StopButton)
	{
		StopButton->OnClicked.AddUniqueDynamic(this, &UAlarmTimingWidget::OnStopButtonClicked);
	}
}

void UAlarmTimingWidget::OnStopButtonClicked(UMenuButton* Button)
{
	// TODO: タイミング判定実装時に TryStopAlarm 相当へ差し替える
	FinishMinigame(true);
}
