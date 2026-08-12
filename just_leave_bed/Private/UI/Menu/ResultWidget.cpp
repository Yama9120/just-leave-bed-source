// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/ResultWidget.h"
#include "UI/Menu/MenuButton.h"
#include "System/RunTracker.h"

void UResultWidget::SetInitialFocus()
{
	if (ButtonReStart)
	{
		ButtonReStart->FocusInnerButton();
	}
}

void UResultWidget::OnButtonReStartClicked(UMenuButton* Button)
{
	Super::OnButtonReStartClicked(Button);

	// クリア時に時間とリトライ回数をリセットする
	if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
	{
		Tracker->ResetRun();
	}
}
