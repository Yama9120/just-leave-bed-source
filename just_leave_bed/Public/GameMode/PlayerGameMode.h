// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameMode.generated.h"

class UStartWidget;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API APlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStartWidget> StartWidgetClass;

private:
	UPROPERTY()
	UStartWidget* StartWidget;
};
