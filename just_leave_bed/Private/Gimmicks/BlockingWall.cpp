// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/BlockingWall.h"

ABlockingWall::ABlockingWall()
{
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);

	// 壁の当たり判定を有効にする
	WallMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ABlockingWall::BeginPlay()
{
	Super::BeginPlay();

	// ギミックが開始される前は壁を非表示にする
	SetWallVisible(false);

	// ターゲットのギミックが設定されていない場合もあるので、ない時はつながずに終わる
	if (!TargetGimmick)
	{
		UE_LOG(LogTemp, Log, TEXT("BlockingWall: %s has no target gimmick."), *GetName());
		return;
	}

	// ターゲットのギミックが完了したときに呼ばれるデリゲートを登録する
	TargetGimmick->OnCompleted.AddDynamic(this, &ABlockingWall::HandleTargetCompleted);
}

void ABlockingWall::Start_Implementation()
{
	// ギミックが開始されたときに壁を表示する
	SetWallVisible(true);

	if (TargetGimmick)
	{
		// ターゲットのギミックを有効にする
		TargetGimmick->SetGimmickEnabled(true);
	}

	UE_LOG(LogTemp, Log, TEXT("BlockingWall: %s shown."), *GetName());
}

void ABlockingWall::StopGimmick_Implementation()
{
	// ギミックが終了したときに壁を非表示にする
	SetWallVisible(false);
}

void ABlockingWall::HandleTargetCompleted()
{
	// ターゲットのギミックが完了したときに壁を非表示にする
	SetWallVisible(false);
	UE_LOG(LogTemp, Log, TEXT("BlockingWall: %s hidden (target completed)."), *GetName());
}

void ABlockingWall::SetWallVisible(bool bVisible)
{
	// 壁の表示状態を設定する
	WallMesh->SetVisibility(bVisible);

	if (bVisible)
	{
		// 壁を表示する場合、当たり判定を有効にする
		WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		// 壁を非表示にする場合、当たり判定を無効にする
		WallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
