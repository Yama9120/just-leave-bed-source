// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Goal.h"
#include "GameMode/PlayerGameMode.h"
#include "Kismet/GameplayStatics.h"

void AGoal::OnPlayerOverlap(APlayerCharacter* Player)
{
	// ゴールに到達した場合の処理
	APlayerGameMode* GameMode = Cast<APlayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// ゴールに到達したことをゲームモードに通知
		GameMode->ShowResultWidget();
	}
}
