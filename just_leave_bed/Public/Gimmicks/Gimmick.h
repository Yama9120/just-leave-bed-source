// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "Gimmick.generated.h"

/**
 *
 */
UCLASS(Abstract)
class JUST_LEAVE_BED_API AGimmick : public APlayerTrigger
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintNativeEvent)
	void Start();
	virtual void Start_Implementation();

	virtual void OnPlayerEnter(APlayerCharacter* Player) override final;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bRunning = false;
};
