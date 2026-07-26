// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/MenuButton.h"
#include "Components/Button.h"

void UMenuButton::FocusInnerButton()
{
	if (InnerButton)
	{
		InnerButton->SetKeyboardFocus();
	}
}

void UMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (InnerButton)
	{
		InnerButton->OnClicked.AddDynamic(this, &UMenuButton::HandleInnerClicked);
	}
}

void UMenuButton::HandleInnerClicked()
{
	OnClicked.Broadcast(this);
}
