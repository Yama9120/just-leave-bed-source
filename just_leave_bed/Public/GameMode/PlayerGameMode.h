// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePhase.h"
#include "UI/Menu/MenuTypes.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameMode.generated.h"

class UStartWidget;
class UResultWidget;
class UDreamGameWidget;
class UInputMappingContext;
class AMyPlayerController;
class UEnhancedInputLocalPlayerSubsystem;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API APlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ShowResultWidget();
	void SetPhaseWidgetInteraction(bool bEnabled);

	void SetPhase(EGamePhase NewPhase);
	EGamePhase GetCurrentPhase() const;
	FInputProfile GetInputProfileForPhase(EGamePhase Phase) const;
	bool CanPause() const;
	void EnterDreamGame();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Phase")
	TMap<EGamePhase, FInputProfile> PhaseInputProfiles;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UDreamGameWidget> DreamGameWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStartWidget> StartWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UResultWidget> ResultWidgetClass;

private:
	void HandleDreamGameCompleted();
	AMyPlayerController* GetMyPlayerController() const;

	UPROPERTY()
	TObjectPtr<UDreamGameWidget> ActiveDreamGameWidget;

	EGamePhase CurrentPhase = EGamePhase::EGP_Title;
};
