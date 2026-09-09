// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "Goal.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API AGoal : public ABaseItem
{
	GENERATED_BODY()

protected:
	virtual void OnPlayerOverlap(APlayerCharacter* Player);
};
