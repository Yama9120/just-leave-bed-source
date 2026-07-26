// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/ResultWidget.h"
#include "UI/Menu/MenuButton.h"

void UResultWidget::SetInitialFocus()
{
	if (ButtonReStart)
	{
		ButtonReStart->SetKeyboardFocus();
	}
}
