// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinigameBase.h"
#include "DreamGameWidget.generated.h"

class UMenuButton;
class UOverlay;

// マルチキャストデリゲートの宣言（引数なし→クリアはtrue、失敗はfalseを返すようにするために引数ありへ）
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDreamGameFinished, bool)

	/**
	 *
	 */
	UCLASS(Abstract)
class JUST_LEAVE_BED_API UDreamGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void FocusCompletedButton();

	UFUNCTION()
	virtual void OnButtonCompleted(UMenuButton* Button);

	// 2Dゲームフェーズ全体の結果(成否)を3D側へ通知する。
	// フェーズが終了したかどうかの判断は呼び出し側(HandleMinigameFinished等)が行い、ここでは判断しない
	UFUNCTION(BlueprintCallable, Category = "DreamGame")
	void NotifyFinished(bool bSuccess);

	FOnDreamGameFinished OnDreamGameFinished;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButton> Completed;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> MinigameSlot;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMinigameBase> MinigameClass;
	UPROPERTY()
	TObjectPtr<UMinigameBase> CurrentMinigame;

	void LaunchMinigame();
	UFUNCTION()
	void HandleMinigameFinished(bool bSuccess);
};
