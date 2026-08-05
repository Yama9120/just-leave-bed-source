// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DreamGame/DreamGameWidget.h"
#include "UI/Menu/MenuButton.h"

void UDreamGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Completed)
	{
		Completed->OnClicked.AddUniqueDynamic(this, &UDreamGameWidget::OnButtonCompleted);
		Completed->FocusInnerButton();
	}
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
	NotifyCompleted();
}

void UDreamGameWidget::NotifyCompleted()
{
	// 2Dゲームフェーズの終了を通知する。個々のミニゲームのクリアでは呼ばない
	OnDreamGameCompleted.Broadcast();
}
