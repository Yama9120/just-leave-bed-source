// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "Checkpoint.generated.h"

class UArrowComponent;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API ACheckpoint : public APlayerTrigger
{
	GENERATED_BODY()

public:
	ACheckpoint();

protected:
	virtual void OnPlayerEnter(APlayerCharacter* Player) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> RespawnArrow;

private:
	FTransform CalculateRespawnTransform(APlayerCharacter* Player);

	float FloorTraceDistance = 500.0f;
	bool bIsReached = false;
};
