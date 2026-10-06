// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "StageTransitionTrigger.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API AStageTransitionTrigger : public APlayerTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Stage")
	FName TargetStageName;
	UPROPERTY(EditAnywhere, Category = "Stage")
	bool bIsPreload = true;

protected:
	virtual void OnPlayerEnter(APlayerCharacter* Player) override;
};
