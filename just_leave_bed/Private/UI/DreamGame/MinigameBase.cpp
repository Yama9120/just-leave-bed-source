// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DreamGame/MinigameBase.h"

// _Implementationを付けることで、BPでオーバーライド可能な関数として定義することができる
void UMinigameBase::StartMinigame_Implementation()
{
}

void UMinigameBase::FinishMinigame(bool bSuccess)
{
	// ミニゲームが終了したことを通知する
	OnMinigameFinished.Broadcast(bSuccess);
}
