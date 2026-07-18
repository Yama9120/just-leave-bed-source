// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameMode.generated.h"

class UStartWidget;
class UResultWidget;
class UBaseWidget;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API APlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ShowResultWidget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStartWidget> StartWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UResultWidget> ResultWidgetClass;
};
