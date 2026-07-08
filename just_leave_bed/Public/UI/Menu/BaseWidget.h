// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/MyPlayerController.h"
#include "BaseWidget.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// このウィジェットが開かれたとき、どの入力モードに切り替えるかを指定
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	EMenuInputMode DesiredInputMode = EMenuInputMode::UIOnly;
	// このウィジェットが開かれたとき、ゲームを止めるかどうかを指定
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	bool bPauseGameWhileOpen = false;

protected:
	virtual void NativeConstruct() override;
	void SetUIMode();
	void SetGameMode();

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ButtonPlay;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ButtonQuit;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ButtonReStart;

	UFUNCTION()
	virtual void OnButtonPlayClicked();
	UFUNCTION()
	virtual void OnButtonQuitClicked();
	UFUNCTION()
	virtual void OnButtonReStartClicked();
};
