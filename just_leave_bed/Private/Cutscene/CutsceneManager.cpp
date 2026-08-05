// Fill out your copyright notice in the Description page of Project Settings.

#include "Cutscene/CutsceneManager.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"

UCutsceneManager::UCutsceneManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCutsceneManager::PlayCutscene(ULevelSequence* _LevelSequence)
{
	FMovieSceneSequencePlaybackSettings _Settings;
	ALevelSequenceActor* _OutActor = nullptr;

	// レベルシーケンスプレイヤーを作成
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), _LevelSequence, _Settings, _OutActor);

	if (!LevelSequencePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayCutscene: failed to create LevelSequencePlayer."));
		return;
	}

	// 再生終了時コールバックを購読して再生開始
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::CallBackFinished);

	// 再生開始
	LevelSequencePlayer->Play();
}

void UCutsceneManager::StopCutscene()
{
	// 再生停止
	if (LevelSequencePlayer)
	{
		LevelSequencePlayer.Get()->Stop();
	}
}

bool UCutsceneManager::IsPlaying() const
{
	// 再生中かどうかを返す
	return (LevelSequencePlayer.Get()->IsPlaying());
}

void UCutsceneManager::CallBackFinished()
{
	UE_LOG(LogTemp, Log, TEXT("CallBackFinished."));

	OnCutsceneFinished.Broadcast();
}
