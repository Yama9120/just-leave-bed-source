// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DreamGame/DreamGameWidget.h"
#include "UI/Menu/MenuButton.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

void UDreamGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Completed)
	{
		Completed->OnClicked.AddUniqueDynamic(this, &UDreamGameWidget::OnButtonCompleted);
		Completed->FocusInnerButton();
	}

	// ミニゲームを開始する
	LaunchMinigame();
}

// 初回のみフォーカスが呼ばれる状態だったので、再度フォーカスを呼ぶ関数を作成
void UDreamGameWidget::FocusCompletedButton()
{
	if (Completed)
	{
		Completed->FocusInnerButton();
	}
}

void UDreamGameWidget::OnButtonCompleted(UMenuButton* Button)
{
	// クリアボタンが押されたあとに、ゲームのクリアを通知する
	NotifyFinished(true);
}

void UDreamGameWidget::NotifyFinished(bool bSuccess)
{
	// 2Dゲームフェーズ全体の結果(成否)を3D側へ通知する
	OnDreamGameFinished.Broadcast(bSuccess);
}

// ミニゲームを開始する関数
void UDreamGameWidget::LaunchMinigame()
{
	if (!MinigameClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MinigameClass is not set."));
		return;
	}
	if (!MinigameSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("MinigameSlot is not set."));
		return;
	}

	CurrentMinigame = CreateWidget<UMinigameBase>(this, MinigameClass);
	if (!CurrentMinigame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create minigame widget."));
		return;
	}

	// 正確な位置合わせのために、OverlaySlotを取得して配置を調整する
	UOverlaySlot* OverlaySlot = MinigameSlot->AddChildToOverlay(CurrentMinigame);
	UE_LOG(LogTemp, Log, TEXT("Minigame %s launched."), *CurrentMinigame->GetName());

	if (OverlaySlot)
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	}

	// ミニゲームの終了を監視する
	CurrentMinigame->OnMinigameFinished.AddUniqueDynamic(this, &UDreamGameWidget::HandleMinigameFinished);

	// ミニゲームを開始する
	CurrentMinigame->StartMinigame();
}

void UDreamGameWidget::HandleMinigameFinished(bool bSuccess)
{
	// 各ミニゲームの終了通知を受け取ったときの処理
	// TODO: 複数ミニゲーム対応時に規定数クリア判定へ差し替え(現状は1本の終了=フェーズ終了として直結)
	NotifyFinished(bSuccess);
}
