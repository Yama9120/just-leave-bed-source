// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/MyPlayerController.h"
#include "UI/Menu/PauseWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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
		PauseWidget->RemoveFromParent();
		FInputModeGameOnly InputMode; // InputModeをGameに切り替える
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		return;
	}

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport();

			// InputModeをGameAndUIに切り替え、ポーズボタンの入力を受け付けるようにする
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;

			// ゲームをポーズする（ゲームの処理を止める）
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}
