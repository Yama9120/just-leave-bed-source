// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputAction;
class UPauseWidget;
class UBaseWidget;

// 入力モードの種類を列挙型で定義
UENUM(BlueprintType)
enum class EMenuInputMode : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI
};

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
	void ApplyMenuInputMode(EMenuInputMode InputMode, bool bPause);

protected:
	virtual void SetupInputComponent() override;

private:
	void PauseGame();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPauseWidget> PauseWidgetClass;

	UPROPERTY()
	UPauseWidget* PauseWidget;

	UPROPERTY()
	UUserWidget* ActiveMenuWidget;
};
