// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PlayerTrigger.h"
#include "Gimmick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGimmickCompleted);

/**
 *
 */
UCLASS(Abstract)
class JUST_LEAVE_BED_API AGimmick : public APlayerTrigger
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void SetGimmickEnabled(bool bNewEnabled);

	UPROPERTY(BlueprintAssignable)
	FOnGimmickCompleted OnCompleted;

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent)
	void Start();
	virtual void Start_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void StopGimmick();
	virtual void StopGimmick_Implementation();
	void CompleteGimmick();

	virtual void OnPlayerEnter(APlayerCharacter* Player) override final;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bRunning = false;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	bool bStartEnabled = true;

private:
	UFUNCTION()
	void HandleSectionEnd();

	UPROPERTY(VisibleInstanceOnly)
	bool bEnabled = true;
};
