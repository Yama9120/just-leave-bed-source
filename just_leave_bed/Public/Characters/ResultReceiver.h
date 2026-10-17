// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ResultReceiver.generated.h"

class UResultEffectTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResult, FGameplayTag, Result);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUST_LEAVE_BED_API UResultReceiver : public UActorComponent
{
	GENERATED_BODY()

public:
	UResultReceiver();

	UFUNCTION(BlueprintCallable)
	void Receive(FGameplayTag Result, FVector Location);

	UPROPERTY(BlueprintAssignable)
	FOnResult OnResult;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UResultEffectTable> Table;
};
