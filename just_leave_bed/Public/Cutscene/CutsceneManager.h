// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CutsceneManager.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneFinished);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUST_LEAVE_BED_API UCutsceneManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCutsceneManager();

	// デモを再生する
	UFUNCTION(BlueprintCallable)
	void PlayCutscene(ULevelSequence* _LevelSequence);

	// デモを停止する
	UFUNCTION(BlueprintCallable)
	void StopCutscene();

	// 再生中か？
	UFUNCTION(BlueprintCallable)
	bool IsPlaying() const;

	UPROPERTY(BlueprintAssignable)
	FOnCutsceneFinished OnCutsceneFinished;

protected:
	// 再生終了時コールバック
	UFUNCTION()
	void CallBackFinished();

	// 再生中のレベルシーケンスプレイヤー(再生中の生存保証を兼ねる)
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
};
