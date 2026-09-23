// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DreamGame/MinigameBase.h"

// BlueprintNativeEventのC++既定実装。BPで上書きされない場合に呼ばれる。
// 基底では空に保つ(BP上書き時に親呼び出しが省略され得るため、共通初期化は置かない)
void UMinigameBase::StartMinigame_Implementation()
{
}

void UMinigameBase::FinishMinigame(bool bSuccess)
{
	// ミニゲームが終了したことを通知する
	OnMinigameFinished.Broadcast(bSuccess);
}
