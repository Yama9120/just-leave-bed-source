// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPoint.generated.h"

class AMovingObject;
class UArrowComponent;

UCLASS()
class JUST_LEAVE_BED_API ALaunchPoint : public AActor
{
	GENERATED_BODY()

public:
	ALaunchPoint();

	UFUNCTION(BlueprintCallable)
	void Fire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMovingObject> ObjectClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> Muzzle;
};
