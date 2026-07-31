// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DreamGameWidget.generated.h"

class UMenuButton;

// マルチキャストデリゲートの宣言（引数なし）
DECLARE_MULTICAST_DELEGATE(FOnDreamGameCompleted);

/**
 *
 */
UCLASS(Abstract)
class JUST_LEAVE_BED_API UDreamGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnButtonCompleted(UMenuButton* Button);

	// クリアしたことを通知する関数
	UFUNCTION(BlueprintCallable, Category = "DreamGame")
	void NotifyCompleted();

	FOnDreamGameCompleted OnDreamGameCompleted;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMenuButton> Completed;
};
