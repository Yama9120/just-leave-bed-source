// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/EquipHolder.h"
#include "Gimmicks/EquipItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

UEquipHolder::UEquipHolder()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipHolder::TryEquip()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// 範囲外で押した直後は無視する（連打防止）
	if (CurrentTime - LastMissTime < MissCoolTime)
	{
		UE_LOG(LogTemp, Log, TEXT("EquipHolder: Equip button pressed too soon after last miss. Cooldown not finished."));
		return;
	}

	AEquipItem* NearestItem = FindNearestItem();

	if (!NearestItem)
	{
		// 見つからなかったときだけ待ち時間を付ける（装備できる場面では待たせない）
		LastMissTime = CurrentTime;
		UE_LOG(LogTemp, Log, TEXT("EquipHolder: No item found within equip radius."));
	}
	else
	{
		// 見つけた装備品と距離をログに出す（EquipRadius の調整用）
		UE_LOG(LogTemp, Log, TEXT("EquipHolder: Found item %s at distance %.2f"), *NearestItem->GetName(), FVector::Dist(GetOwner()->GetActorLocation(), NearestItem->GetActorLocation()));
	}
}

AEquipItem* UEquipHolder::FindNearestItem() const
{
	FVector CharacterLocation = GetOwner()->GetActorLocation();

	// 装備品の Object Type（WorldDynamic）だけを探す
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	// 自分（キャラ）は探す対象から外す
	TArray<AActor*> IgnoreActors = {GetOwner()};

	TArray<AActor*> OutActors;

	// キャラを中心に、半径 EquipRadius の球に重なっている AEquipItem を集める
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CharacterLocation, EquipRadius, ObjectTypes, AEquipItem::StaticClass(), IgnoreActors, OutActors);

	// ---- 半径の調整用。調整が終わったら消す ----
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), EquipRadius, 16, FColor::Green, false, 1.f);
	// ---- 半径の調整用。調整が終わったら消す ----

	// 集めた中からキャラに一番近い装備品を選ぶ
	AEquipItem* NearestItem = nullptr;
	float NearestDistSquared = TNumericLimits<float>::Max();

	for (AActor* Actor : OutActors)
	{
		AEquipItem* EquipItem = Cast<AEquipItem>(Actor);
		if (EquipItem)
		{
			// 比べるだけなので平方根を取らない DistSquared を使う
			float DistSquared = FVector::DistSquared(CharacterLocation, EquipItem->GetActorLocation());
			if (DistSquared < NearestDistSquared)
			{
				NearestDistSquared = DistSquared;
				NearestItem = EquipItem;
			}
		}
	}

	return NearestItem;
}
