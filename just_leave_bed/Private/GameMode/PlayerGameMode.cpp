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
}

FInputProfile APlayerGameMode::GetInputProfileForPhase(EGamePhase Phase) const
{
	// Phaseに対応する入力プロファイルを取得する
	if (const FInputProfile* Found = PhaseInputProfiles.Find(Phase)) // Find関数は、指定されたキーに対応する値へのポインタを返す
	{
		return *Found;
	}

	// もし見つからなかった場合はデフォルトの入力プロファイルを返す
	UE_LOG(LogTemp, Warning, TEXT("Input profile for phase %s not found."), *UEnum::GetValueAsString(Phase));
	return FInputProfile();
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

	// 変更後のフェーズに応じた入力プロファイルを適用する
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->RefreshInputProfile();
	}
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
