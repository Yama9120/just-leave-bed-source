// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/MenuButton.h"
#include "UI/DreamGame/DreamGameWidget.h"

void UDreamGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Completed)
	{
		Completed->OnClicked.AddUniqueDynamic(this, &UDreamGameWidget::OnButtonCompleted);
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
