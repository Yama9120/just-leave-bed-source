// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/ResultWidget.h"
#include "UI/Menu/MenuButton.h"
#include "System/RunTracker.h"
#include "Components/TextBlock.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// リザルト画面が開かれたときに、RunTrackerから時間とリトライ回数を取得して表示する
	if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
	{
		SetResult(Tracker->GetRunTime(), Tracker->GetRetryCount());
	}
}

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

	// クリア時に時間とリトライ回数をリセットする（リトライ時はリトライ回数を加算）
	if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
	{
		Tracker->ResetRun();
	}
}

void UResultWidget::SetResult(float Time, int32 RetryCount)
{
	// リザルト画面に時間とリトライ回数を表示する
	TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.2f"), Time)));
	RetryCountText->SetText(FText::FromString(FString::Printf(TEXT("Retry: %d回"), RetryCount)));
}
