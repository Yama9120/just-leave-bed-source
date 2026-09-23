// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinigameBase.generated.h"

// 各ミニゲームが終了したことを通知するデリゲートの宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinigameFinished, bool, bSuccess);

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UMinigameBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMinigameFinished OnMinigameFinished;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartMinigame();
	virtual void StartMinigame_Implementation();
	UFUNCTION(BlueprintCallable)
	void FinishMinigame(bool bSuccess);
};
