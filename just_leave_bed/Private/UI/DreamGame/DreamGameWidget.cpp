// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/MenuButton.h"
#include "UI/DreamGame/DreamGameWidget.h"

void UDreamGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Completed)
	{
		Completed->OnClicked.AddUniqueDynamic(this, &UDreamGameWidget::OnButtonCompleted);
	}
}

void UDreamGameWidget::OnButtonCompleted(UMenuButton* Button)
{
	// クリアボタンが押されたときの処理をここに記述
	NotifyCompleted();
}

void UDreamGameWidget::NotifyCompleted()
{
	// GameModeがOnDreamGameCompletedイベントを受け取るようにする
	OnDreamGameCompleted.Broadcast();
}
