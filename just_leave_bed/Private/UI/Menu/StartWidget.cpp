// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/StartWidget.h"
#include "GameMode/PlayerGameMode.h"

void UStartWidget::OnButtonPlayClicked(UMenuButton* Button)
{
	// ゲームのフェーズを3Dゲームに変更する
	if (APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>())
	{
		GameMode->SetPhase(EGamePhase::EGP_3DGame);
	}

	Super::OnButtonPlayClicked(Button);
}
