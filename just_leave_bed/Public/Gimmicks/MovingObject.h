// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "GameplayTagContainer.h"
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
	virtual void OnPlayerEnter(APlayerCharacter* Player) override;
	UFUNCTION()
	virtual void OnStop(const FHitResult& HitResult);
	void Notify(FGameplayTag Tag);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Object;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> Movement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (Units = "s"))
	float Lifespan = 5.0f; // 0.0f なら時間では消えない

private:
	bool bNotified = false; // すでに通知済みかどうかを追跡するフラグ
};
