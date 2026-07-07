// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Goal.h"
#include "Characters/PlayerCharacter.h"

void AGoal::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 重なったアクターがプレイヤーキャラクターかどうかを確認（例：敵が重なったらゴール、を防ぐ）
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		const FString OtherActorName = OtherActor->GetName();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Goal: %s"), *OtherActorName));
		}
	}
}
