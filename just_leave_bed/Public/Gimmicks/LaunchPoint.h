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
	UFUNCTION(BlueprintCallable)
	void Launch();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMovingObject> ObjectClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f))
	float Delay = 0.0f; // 発射までの遅延時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f))
	float Interval = 0.0f; // 発射間隔
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Count = 0; // 発射回数

private:
	void OnTimer();

	FTimerHandle Timer;
	int32 FireCount;
};
