// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/PauseWidget.h"
#include "System/RunTracker.h"

void UPauseWidget::OnButtonReStartClicked(UMenuButton* Button)
{
	Super::OnButtonReStartClicked(Button);

	// リトライ回数を加算する（クリア時はリセット）
	if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
	{
		Tracker->AddRetry();
	}
}