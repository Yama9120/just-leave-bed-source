// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmicks/MovingObject.h"
#include "EquipItem.generated.h"

/**
 *
 */
UCLASS(Abstract)
class JUST_LEAVE_BED_API AEquipItem : public AMovingObject
{
	GENERATED_BODY()

protected:
	virtual void OnStop(const FHitResult& HitResult) override;
	virtual void LifeSpanExpired() override;
};
