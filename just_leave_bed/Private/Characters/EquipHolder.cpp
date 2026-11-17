// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/EquipHolder.h"
#include "Characters/PlayerCharacter.h"
#include "Gimmicks/EquipItem.h"
#include "System/RetrySubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

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
		// 見た目を付けられたら、装備品に Result.Equip を通知して消す
		if (AttachItem(NearestItem))
		{
			NearestItem->FinishEquip();
		}
		// 付けられなかったとき（設定ミス）は装備したことにせず、そのまま飛ばす
	}
}

void UEquipHolder::BeginPlay()
{
	Super::BeginPlay();

	URetrySubsystem* RetrySubsystem = GetWorld()->GetSubsystem<URetrySubsystem>();
	if (!RetrySubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipHolder: RetrySubsystem not found."));
		return;
	}

	RetrySubsystem->OnRetryStarted.AddDynamic(this, &UEquipHolder::HandleRetryStarted);
	RetrySubsystem->OnCheckpointReached.AddDynamic(this, &UEquipHolder::HandleCheckpointReached);
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

bool UEquipHolder::AttachItem(AEquipItem* Item)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipHolder: Owner is not a PlayerCharacter."));
		return false;
	}

	// マネキンではなく見た目の体に付ける
	USkeletalMeshComponent* VisualMesh = PlayerCharacter->GetVisualMesh();
	if (!VisualMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipHolder: PlayerCharacter has no VisualMesh."));
		return false;
	}

	UStaticMesh* EquipMesh = Item->GetEquipMesh();
	if (!EquipMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipHolder: %s has no mesh to equip."), *Item->GetName());
		return false;
	}

	FName Socket = Item->GetAttachSocket();
	if (!VisualMesh->DoesSocketExist(Socket))
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipHolder: Socket %s not found on VisualMesh."), *Socket.ToString());
		return false;
	}

	// 飛んできた物体はギミック側で消されるため、キャラ側に新しく見た目の部品を作って付ける
	UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(PlayerCharacter);
	NewMeshComp->SetStaticMesh(EquipMesh);
	// 付けた装備が他のものに当たらないようにする
	NewMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ソケットで調整した大きさも反映させる（スケルトンエディタのプレビューと同じ見た目にする）
	// （同じソケットに付くすべてのアイテムのスケールに影響があるが、現状頭のソケットに付くのは帽子だけなので問題ない）
	NewMeshComp->AttachToComponent(VisualMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
	// 実行中に作った部品は登録しないと表示されない
	NewMeshComp->RegisterComponent();

	// PendingMeshes に保持しておくことで、リトライ時に消すことができる
	PendingMeshes.Add(NewMeshComp);

	UE_LOG(LogTemp, Log, TEXT("EquipHolder: Equipped %s to %s."), *Item->GetName(), *Socket.ToString());
	return true;
}

void UEquipHolder::HandleRetryStarted()
{
	UE_LOG(LogTemp, Log, TEXT("EquipHolder: Retry started. Removing %d pending item(s)."), PendingMeshes.Num());

	// リトライ時に PendingMeshes に保持している装備品の見た目を消す
	for (UStaticMeshComponent* MeshComp : PendingMeshes)
	{
		if (MeshComp)
		{
			MeshComp->DestroyComponent();
		}
	}
	PendingMeshes.Empty();
}

void UEquipHolder::HandleCheckpointReached()
{
	UE_LOG(LogTemp, Log, TEXT("EquipHolder: Checkpoint reached. Confirming %d item(s)."), PendingMeshes.Num());

	// 確定した装備は一覧から外すだけで、キャラには付けたまま
	PendingMeshes.Empty();
}
