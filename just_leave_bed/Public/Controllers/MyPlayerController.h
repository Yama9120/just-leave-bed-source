// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputAction;
class UPauseWidget;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPauseWidget> PauseWidgetClass;

private:
	UPROPERTY()
	UPauseWidget* PauseWidget;

private:
	void PauseGame();
};
