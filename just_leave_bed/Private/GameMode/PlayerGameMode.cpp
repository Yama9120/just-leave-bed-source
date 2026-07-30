// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/PlayerGameMode.h"
#include "UI/Menu/StartWidget.h"
#include "UI/Menu/ResultWidget.h"
#include "Controllers/MyPlayerController.h"

void APlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (StartWidgetClass)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OpenMenuWidget(StartWidgetClass);
		}
	}

	// Debug用
	SetPhase(EGamePhase::EGP_2DGame);
}

void APlayerGameMode::SetPhase(EGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}

	CurrentPhase = NewPhase;

	// Debug用
	UE_LOG(LogTemp, Log, TEXT("%s"), *UEnum::GetValueAsString(CurrentPhase));
}

EGamePhase APlayerGameMode::GetCurrentPhase() const
{
	return CurrentPhase;
}

bool APlayerGameMode::CanPause() const
{
	return CurrentPhase != EGamePhase::EGP_Result;
}

void APlayerGameMode::ShowResultWidget()
{
	if (ResultWidgetClass)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OpenMenuWidget(ResultWidgetClass);
		}
	}
}
