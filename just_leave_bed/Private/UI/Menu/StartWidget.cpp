// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/StartWidget.h"
#include "Components/Button.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UStartWidget::OnButtonPlayClicked);
}

void UStartWidget::OnButtonPlayClicked()
{
}
