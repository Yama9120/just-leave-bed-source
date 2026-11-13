// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipHolder.generated.h"

class AEquipItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUST_LEAVE_BED_API UEquipHolder : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipHolder();

	UFUNCTION(BlueprintCallable, Category = "Equip")
	void TryEquip();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Equip")
	float EquipRadius = 150.f; // 装備可能な距離
	UPROPERTY(EditAnywhere, Category = "Equip")
	float MissCoolTime = 1.0f; // 装備ボタン連打防止のためのクールタイム

private:
	AEquipItem* FindNearestItem() const;
	bool AttachItem(AEquipItem* Item);

	UFUNCTION()
	void HandleRetryStarted();
	UFUNCTION()
	void HandleCheckpointReached();

	float LastMissTime = 0.f; // 最後に範囲外で装備しようとした時刻

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> PendingMeshes; // 装備中のメッシュを一時的に保持する配列
};
