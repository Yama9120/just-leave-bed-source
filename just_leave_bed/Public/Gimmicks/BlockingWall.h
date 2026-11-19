// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmicks/Gimmick.h"
#include "BlockingWall.generated.h"

/**
 *
 */
UCLASS()
class JUST_LEAVE_BED_API ABlockingWall : public AGimmick
{
	GENERATED_BODY()

public:
	ABlockingWall();

protected:
	virtual void BeginPlay() override;
	virtual void Start_Implementation() override;
	virtual void StopGimmick_Implementation() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WallMesh;
	UPROPERTY(EditInstanceOnly, Category = "Gimmick")
	TObjectPtr<AGimmick> TargetGimmick;
	UPROPERTY(EditAnywhere, Category = "Gimmick")
	bool bHideOnSectionEnd = true; // クリアしたステージにもどれないようにするために、ギミックの区間が終わったら壁を非表示にするかどうかの設定

private:
	UFUNCTION()
	void HandleTargetCompleted();
	void SetWallVisible(bool bVisible);
};
