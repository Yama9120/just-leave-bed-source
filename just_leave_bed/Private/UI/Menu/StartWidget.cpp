// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/StartWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UStartWidget::OnButtonPlayClicked);

	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UStartWidget::OnButtonQuitClicked);
}

void UStartWidget::OnButtonPlayClicked()
{
	// Widget‚Ì”ñ•\¦
	RemoveFromParent();

	// InputMode‚ğGame‚ÉØ‚è‘Ö‚¦‚é
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UStartWidget::OnButtonQuitClicked()
{
	// PlayerController‚ğæ“¾‚µ‚ÄƒQ[ƒ€‚ğI—¹‚·‚é
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
