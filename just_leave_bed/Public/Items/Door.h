// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

class UCurveFloat;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API ADoor : public APlayerTrigger
{
	GENERATED_BODY()

public:
	ADoor();

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorTimelineFloatCurve;

protected:
	virtual void BeginPlay() override;
	virtual void OnPlayerEnter(APlayerCharacter* Player) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DoorHinge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* DoorTimelineComp;

private:
	UFUNCTION()
	void UpdateTimelineComp(float Output);
	void CloseDoor();

	FOnTimelineFloat UpdateFunctionFloat;

	UPROPERTY(EditAnywhere)
	float DoorCloseDuration = 4.0f;
	FTimerHandle DoorCloseTimerHandle;
};
