// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ResultEffectTable.generated.h"

class UNiagaraSystem;

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API UResultEffectTable : public UDataAsset
{
	GENERATED_BODY()

public:
	UNiagaraSystem* Find(FGameplayTag Result) const;

private:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TObjectPtr<UNiagaraSystem>> Effects;
};
