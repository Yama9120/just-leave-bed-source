// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void SetUIMode();
	void SetGameMode();

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ButtonPlay;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ButtonQuit;

	UFUNCTION()
	virtual void OnButtonPlayClicked();
	UFUNCTION()
	virtual void OnButtonQuitClicked();
};
