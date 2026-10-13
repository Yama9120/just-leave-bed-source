// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "MovingObject.generated.h"

class UProjectileMovementComponent;

/**
 *
 */
UCLASS(Abstract)
class JUST_LEAVE_BED_API AMovingObject : public APlayerTrigger
{
	GENERATED_BODY()

public:
	AMovingObject();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStop(const FHitResult& HitResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Object;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> Movement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Lifespan = 5.0f;
};
