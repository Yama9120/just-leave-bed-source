// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/BaseWidget.h"
#include "PauseWidget.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UPauseWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void OnButtonReStartClicked(UMenuButton* Button);
};
