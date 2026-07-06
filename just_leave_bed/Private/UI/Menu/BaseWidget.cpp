// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/BaseWidget.h"
#include "Components/Button.h"
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
}

void UBaseWidget::OnButtonPlayClicked()
{
	// Widgetの非表示
	RemoveFromParent();

	// InputModeをGameに切り替える
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
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