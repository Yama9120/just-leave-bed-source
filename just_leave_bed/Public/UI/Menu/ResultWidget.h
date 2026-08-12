// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/BaseWidget.h"
#include "ResultWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UResultWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	void SetResult(float Time, int32 RetryCount);

protected:
	virtual void NativeConstruct() override;
	virtual void SetInitialFocus() override;
	virtual void OnButtonReStartClicked(UMenuButton* Button) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RetryCountText;
};
