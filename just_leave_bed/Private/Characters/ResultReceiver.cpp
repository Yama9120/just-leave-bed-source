// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ResultReceiver.h"
#include "Gimmicks/ResultEffectTable.h"
#include "NiagaraFunctionLibrary.h"

UResultReceiver::UResultReceiver()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResultReceiver::Receive(FGameplayTag Result, FVector Location)
{
	if (Table)
	{
		// タグに対応するエフェクトをテーブルから取得し、存在すればスポーンする
		UNiagaraSystem* Effect = Table->Find(Result);
		if (Effect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect, Location);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ResultReceiver: No effect for %s in %s."), *Result.ToString(), *GetNameSafe(Table));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ResultReceiver: Table is not set on %s."), *GetNameSafe(GetOwner()));
	}

	// 演出の有無に関係なく、結果は必ず通知する
	OnResult.Broadcast(Result);
}
