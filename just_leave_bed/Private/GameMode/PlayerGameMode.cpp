// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/PlayerGameMode.h"
#include "UI/Menu/StartWidget.h"
#include "UI/Menu/ResultWidget.h"

void APlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (StartWidgetClass)
	{
		StartWidget = CreateWidget<UStartWidget>(GetWorld(), StartWidgetClass);
		if (StartWidget)
		{
			StartWidget->AddToViewport();
		}
	}
}

void APlayerGameMode::ShowResultWidget()
{
	if (ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);
		if (ResultWidget)
		{
			ResultWidget->AddToViewport();
		}
	}
}
