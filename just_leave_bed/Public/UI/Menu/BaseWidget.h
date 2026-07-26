// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Menu/MenuTypes.h"
#include "BaseWidget.generated.h"

class UMenuButton;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// このウィジェットが開かれたとき、どの入力モードに切り替えるかを指定
	// UIOnly: マウス操作のみの画面用（スタート・リザルト）
	// GameAndUI: ゲーム側の入力も受け付ける画面用（ポーズ：ポーズキーで画面を閉じられるようにする）
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	EMenuInputMode DesiredInputMode = EMenuInputMode::UIOnly;
	// このウィジェットが開かれたとき、ゲームを止めるかどうかを指定
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	bool bPauseGameWhileOpen = false;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButton> ButtonPlay;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButton> ButtonQuit;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButton> ButtonReStart;

	UFUNCTION()
	virtual void OnButtonPlayClicked(UMenuButton* Button);
	UFUNCTION()
	virtual void OnButtonQuitClicked(UMenuButton* Button);
	UFUNCTION()
	virtual void OnButtonReStartClicked(UMenuButton* Button);

	UFUNCTION()
	virtual void SetInitialFocus();
};
