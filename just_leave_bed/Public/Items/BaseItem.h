// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseItem.generated.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

UCLASS()
class JUST_LEAVE_BED_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
