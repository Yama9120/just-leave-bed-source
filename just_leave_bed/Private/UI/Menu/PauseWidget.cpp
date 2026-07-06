// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/PauseWidget.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::OnButtonReStartClicked()
{
	// ゲームのポーズを解除する（止めていたゲームの再開）
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// 現在のレベル名を取得（今回は一つのレベルで行うので、現在のレベルが分かれば良い）
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 現在のレベルを再読み込みする
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
}
