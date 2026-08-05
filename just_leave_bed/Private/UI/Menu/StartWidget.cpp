// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/StartWidget.h"
#include "GameMode/PlayerGameMode.h"

void UStartWidget::OnButtonPlayClicked(UMenuButton* Button)
{
	Super::OnButtonPlayClicked(Button);

	// ゲームのフェーズを3Dゲームに変更する
	if (APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>())
	{
		// スタート画面でスタートボタンを押したときに、イントロムービーを再生する
		GameMode->StartIntroCutscene();
	}
}
