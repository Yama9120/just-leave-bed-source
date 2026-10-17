// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/EquipItem.h"
#include "Gimmicks/ResultTags.h"

void AEquipItem::OnStop(const FHitResult& HitResult)
{
	// 装備されずに壁や地面に当たった場合は、失敗通知を行う
	// 結果を通知してからオブジェクトを破棄する
	Notify(ResultTags::FailMiss);
	Super::OnStop(HitResult);
}

void AEquipItem::LifeSpanExpired()
{
	// 寿命が尽きた場合も、同様に失敗通知を行う
	Notify(ResultTags::FailMiss);
	Super::LifeSpanExpired();
}
