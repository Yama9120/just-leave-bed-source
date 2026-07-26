// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/BaseWidget.h"
#include "Components/Button.h"
#include "Controllers/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonPlay)
	{
		ButtonPlay->OnClicked.AddUniqueDynamic(this, &UBaseWidget::OnButtonPlayClicked);
	}
	if (ButtonQuit)
	{
		ButtonQuit->OnClicked.AddUniqueDynamic(this, &UBaseWidget::OnButtonQuitClicked);
	}
	if (ButtonReStart)
	{
		ButtonReStart->OnClicked.AddUniqueDynamic(this, &UBaseWidget::OnButtonReStartClicked);
	}

	SetInitialFocus();
}

void UBaseWidget::OnButtonPlayClicked()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->CloseMenuWidget();
	}
}

void UBaseWidget::OnButtonQuitClicked()
{
	// PlayerControllerを取得してゲームを終了する
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

void UBaseWidget::OnButtonReStartClicked()
{
	// ゲームのポーズを解除する（止めていたゲームの再開）
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// 現在のレベル名を取得（今回は一つのレベルで行うので、現在のレベルが分かれば良い）
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 現在のレベルを再読み込みする
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
}

void UBaseWidget::SetInitialFocus()
{
	if (ButtonPlay)
	{
		ButtonPlay->SetKeyboardFocus();
	}
}
