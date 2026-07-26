// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void FocusInnerButton();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuButtonClicked, UMenuButton*, Button);
	FOnMenuButtonClicked OnClicked;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* InnerButton;

	UFUNCTION()
	void HandleInnerClicked();
};
