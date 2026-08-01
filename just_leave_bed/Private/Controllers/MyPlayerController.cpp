// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/MyPlayerController.h"
#include "GameMode/PlayerGameMode.h"
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

	// ゲームの現在のフェーズに応じた入力プロファイルを再適用する
	RefreshInputProfile();
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
	ResetIgnoreMoveInput();
	ResetIgnoreLookInput();
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

void AMyPlayerController::RefreshInputProfile()
{
	// ウィジェットが開いている場合はその入力プロファイルを適用する
	if (ActiveMenuWidget)
	{
		ApplyMenuInputMode(ActiveMenuWidget->InputProfile, ActiveMenuWidget->bPauseGameWhileOpen);
	}
	// 開いていない場合はゲームモードの現在のフェーズに応じた入力プロファイルを適用する
	else
	{
		if (const APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>())
		{
			const EGamePhase CurrentPhase = GameMode->GetCurrentPhase();
			const FInputProfile InputProfile = GameMode->GetInputProfileForPhase(CurrentPhase);
			ApplyMenuInputMode(InputProfile, false);
		}
	}
}

void AMyPlayerController::PauseGame()
{
	// すでにPauseWidgetが開いている場合は閉じる
	if (ActiveMenuWidget && ActiveMenuWidget->IsA(PauseWidgetClass))
	{
		CloseMenuWidget();
		return;
	}

	// すでに他のメニューが開いている場合は何もしない
	if (ActiveMenuWidget)
	{
		return;
	}

	// ゲームモードがポーズ可能かどうかを確認する
	const APlayerGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayerGameMode>();
	if (GameMode && !GameMode->CanPause())
	{
		return;
	}

	OpenMenuWidget(PauseWidgetClass);
}
