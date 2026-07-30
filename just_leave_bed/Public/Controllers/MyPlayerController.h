// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Menu/MenuTypes.h"
#include "MyPlayerController.generated.h"

class UInputAction;
class UPauseWidget;
class UBaseWidget;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UUserWidget* OpenMenuWidget(TSubclassOf<UBaseWidget> WidgetClass);
	void CloseMenuWidget();
	void ApplyMenuInputMode(const FInputProfile& Profile, bool bPause);

protected:
	virtual void SetupInputComponent() override;

private:
	void PauseGame();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPauseWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* ActiveMenuWidget;
};
