// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/FailWidget.h"
#include "UI/Menu/MenuButton.h"
#include "System/RunTracker.h"

void UFailWidget::SetInitialFocus()
{
	if (ButtonReStart)
	{
		ButtonReStart->FocusInnerButton();
	}
}

void UFailWidget::OnButtonReStartClicked(UMenuButton* Button)
{
	Super::OnButtonReStartClicked(Button);

	// リトライ回数を加算する（クリア時はリセット）
	if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
	{
		Tracker->AddRetry();
	}
}
