// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerTrigger.generated.h"

class UBoxComponent;
class APlayerCharacter;

/**
 * プレイヤーが範囲に入った / 出たときに処理を行うアクターの基盤
 */
UCLASS()
class JUST_LEAVE_BED_API APlayerTrigger : public AActor
{
	GENERATED_BODY()

public:
	APlayerTrigger();

protected:
	virtual void BeginPlay() override;

	// プレイヤーが範囲に入ったときに呼ばれる。子クラスでオーバーライドする
	virtual void OnPlayerEnter(APlayerCharacter* Player);
	// プレイヤーが範囲から出たときに呼ばれる。子クラスでオーバーライドする
	virtual void OnPlayerExit(APlayerCharacter* Player);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
