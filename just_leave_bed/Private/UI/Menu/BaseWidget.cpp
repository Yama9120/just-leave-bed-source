// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/BaseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// InputModeをUIに切り替える
	SetUIMode();

	if (ButtonPlay)
	{
		ButtonPlay->OnClicked.AddUniqueDynamic(this, &UBaseWidget::OnButtonPlayClicked);
	}
	if (ButtonQuit)
	{
		ButtonQuit->OnClicked.AddUniqueDynamic(this, &UBaseWidget::OnButtonQuitClicked);
	}
}

void UBaseWidget::SetUIMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputMode; // InputModeをUIに切り替える
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UBaseWidget::SetGameMode()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode; // InputModeをGameに切り替える
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UBaseWidget::OnButtonPlayClicked()
{
	// Widgetの非表示
	RemoveFromParent();

	// InputModeをGameに切り替える
	SetGameMode();

	// ゲームのポーズを解除する（止めていたゲームの再開）
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UBaseWidget::OnButtonQuitClicked()
{
	// PlayerControllerを取得してゲームを終了する
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}