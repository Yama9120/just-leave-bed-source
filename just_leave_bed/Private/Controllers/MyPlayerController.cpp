// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/MyPlayerController.h"
#include "UI/Menu/BaseWidget.h"
#include "UI/Menu/PauseWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

UUserWidget* AMyPlayerController::OpenMenuWidget(TSubclassOf<UBaseWidget> WidgetClass)
{
	// 既に開いている場合は何もしない
	if (ActiveMenuWidget)
	{
		return nullptr;
	}

	if (!WidgetClass)
	{
		return nullptr;
	}

	UBaseWidget* Widget = CreateWidget<UBaseWidget>(this, WidgetClass);
	if (!Widget)
	{
		return nullptr;
	}

	Widget->AddToViewport();
	ActiveMenuWidget = Widget;

	// 入力モード、ポーズ設定を反映する
	ApplyMenuInputMode(Widget->InputProfile, Widget->bPauseGameWhileOpen);

	return Widget;
}

void AMyPlayerController::CloseMenuWidget()
{
	// 既に閉じている場合は何もしない
	if (!ActiveMenuWidget)
	{
		return;
	}

	// ウィジェットを閉じる（取り除く）
	ActiveMenuWidget->RemoveFromParent();
	ActiveMenuWidget = nullptr;

	// 入力モードをGameにし、ポーズを解除する
	ApplyMenuInputMode(FInputProfile(), false);
}

void AMyPlayerController::ApplyMenuInputMode(const FInputProfile& Profile, bool bPause)
{
	switch (Profile.InputMode)
	{
		case EMenuInputMode::EMIM_GameOnly:
		{
			SetInputMode(FInputModeGameOnly());
			break;
		}
		case EMenuInputMode::EMIM_UIOnly:
		{
			SetInputMode(FInputModeUIOnly());
			break;
		}
		case EMenuInputMode::EMIM_GameAndUI:
		{
			SetInputMode(FInputModeGameAndUI());
			break;
		}
	}

	bShowMouseCursor = Profile.bShowCursor;
	SetIgnoreMoveInput(Profile.bIgnoreMoveInput);
	SetIgnoreLookInput(Profile.bIgnoreLookInput);

	// ゲームのポーズ状態を設定する
	UGameplayStatics::SetGamePaused(GetWorld(), bPause);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AMyPlayerController::PauseGame);
	}
}

void AMyPlayerController::PauseGame()
{
	// ゲームがすでにポーズされている場合はポーズ画面を閉じる
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		CloseMenuWidget();
		return;
	}

	OpenMenuWidget(PauseWidgetClass);
}
