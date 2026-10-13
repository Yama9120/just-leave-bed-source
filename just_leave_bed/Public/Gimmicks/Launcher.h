// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmicks/Gimmick.h"
#include "Launcher.generated.h"

class ALaunchPoint;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API ALauncher : public AGimmick
{
	GENERATED_BODY()

protected:
	virtual void Start_Implementation() override;

	UPROPERTY(EditInstanceOnly)
	TArray<TObjectPtr<ALaunchPoint>> LaunchPoints;
};
