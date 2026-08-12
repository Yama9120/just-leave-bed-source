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
class ACineCameraActor;
class UCutsceneManager;
class ULevelSequence;

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
	void StartIntroCutscene();

protected:
	APlayerGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetupTitleCamera();

	UPROPERTY(EditDefaultsOnly, Category = "Phase")
	TMap<EGamePhase, FInputProfile> PhaseInputProfiles;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UDreamGameWidget> DreamGameWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStartWidget> StartWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<ACineCameraActor> TitleCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cutscene")
	TObjectPtr<UCutsceneManager> CutsceneManager;

	UPROPERTY(EditDefaultsOnly, Category = "Cutscene")
	TObjectPtr<ULevelSequence> IntroCutscene;

private:
	void HandleDreamGameCompleted();
	AMyPlayerController* GetMyPlayerController() const;

	UFUNCTION()
	void HandleIntroCutsceneFinished();

	UPROPERTY()
	TObjectPtr<UDreamGameWidget> ActiveDreamGameWidget;

	EGamePhase CurrentPhase = EGamePhase::EGP_Title;
};
