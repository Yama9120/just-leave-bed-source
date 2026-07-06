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
	// ゲームがすでにポーズされている場合は何もしない
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return;
	}

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport();
			// ゲームをポーズする（ゲームの処理を止める）
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}
