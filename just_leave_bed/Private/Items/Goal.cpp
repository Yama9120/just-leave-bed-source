// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Goal.h"
#include "Characters/PlayerCharacter.h"
#include "GameMode/PlayerGameMode.h"
#include "Kismet/GameplayStatics.h"

void AGoal::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 重なったアクターがプレイヤーキャラクターかどうかを確認（例：敵が重なったらゴール、を防ぐ）
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		// ゴールに到達した場合の処理
		APlayerGameMode* GameMode = Cast<APlayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			// ゴールに到達したことをゲームモードに通知
			GameMode->ShowResultWidget();
		}
	}
}
