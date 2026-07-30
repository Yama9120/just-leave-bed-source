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
	// 入力モード・移動/視点操作の有効化・マウスカーソルの表示などをWBP側で設定するのではなく、C++側で設定
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	FInputProfile InputProfile;
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
