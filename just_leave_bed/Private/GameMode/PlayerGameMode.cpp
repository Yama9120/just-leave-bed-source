// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/PlayerGameMode.h"
#include "UI/Menu/StartWidget.h"
#include "UI/Menu/ResultWidget.h"
#include "UI/DreamGame/DreamGameWidget.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/MyPlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "CineCameraActor.h"
#include "EngineUtils.h"
#include "Cutscene/CutsceneManager.h"
#include "System/RunTracker.h"

APlayerGameMode::APlayerGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	CutsceneManager = CreateDefaultSubobject<UCutsceneManager>(TEXT("CutsceneManager"));
}

void APlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupTitleCamera();

	if (StartWidgetClass)
	{
		AMyPlayerController* PlayerController = GetMyPlayerController();
		if (PlayerController)
		{
			PlayerController->OpenMenuWidget(StartWidgetClass);
		}
	}

	if (IntroCutscene)
	{
		CutsceneManager->OnCutsceneFinished.AddDynamic(this, &APlayerGameMode::HandleIntroCutsceneFinished);
	}
}

void APlayerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ゲームフェーズが2Dゲームまたは3Dゲームの場合、RunTrackerにゲーム時間を追加する
	if (CurrentPhase == EGamePhase::EGP_2DGame || CurrentPhase == EGamePhase::EGP_3DGame)
	{
		if (URunTracker* Tracker = GetGameInstance()->GetSubsystem<URunTracker>())
		{
			Tracker->AddGameTime(DeltaSeconds);
		}
	}
}

void APlayerGameMode::SetupTitleCamera()
{
	// レベル内のすべての CineCameraActor を検索し、"TitleCamera" タグを持つものを見つける
	for (TActorIterator<ACineCameraActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("TitleCamera")))
		{
			TitleCamera = *It;
			break;
		}
	}

	if (!TitleCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: TitleCamera not found in level."));
	}
	else
	{
		// タイトルカメラに切り替える処理を次のティックに遅延させる
		GetWorldTimerManager().SetTimerForNextTick([this]() {
			if (AMyPlayerController* PC = GetMyPlayerController())
			{
				// タイトルカメラに切り替える
				PC->SetViewTarget(TitleCamera);
			}
		});
	}
}

FInputProfile APlayerGameMode::GetInputProfileForPhase(EGamePhase Phase) const
{
	// Phaseに対応する入力プロファイルを取得する
	if (const FInputProfile* Found = PhaseInputProfiles.Find(Phase)) // Find関数は、指定されたキーに対応する値へのポインタを返す
	{
		return *Found;
	}

	// もし見つからなかった場合はデフォルトの入力プロファイルを返す
	UE_LOG(LogTemp, Warning, TEXT("Input profile for phase %s not found."), *UEnum::GetValueAsString(Phase));
	return FInputProfile();
}

void APlayerGameMode::SetPhase(EGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}

	CurrentPhase = NewPhase;

	// Debug用
	UE_LOG(LogTemp, Log, TEXT("%s"), *UEnum::GetValueAsString(CurrentPhase));

	// 変更後のフェーズに応じた入力プロファイルを適用する
	AMyPlayerController* PlayerController = GetMyPlayerController();
	if (PlayerController)
	{
		PlayerController->RefreshInputProfile();
	}
}

EGamePhase APlayerGameMode::GetCurrentPhase() const
{
	return CurrentPhase;
}

bool APlayerGameMode::CanPause() const
{
	return CurrentPhase != EGamePhase::EGP_Result;
}

void APlayerGameMode::EnterDreamGame()
{
	// 現在のフェーズが2Dイントロムービーでない場合は何もしない
	if (CurrentPhase != EGamePhase::EGP_2DIntroMovie)
	{
		return;
	}

	if (!DreamGameWidgetClass)
	{
		return;
	}

	AMyPlayerController* PlayerController = GetMyPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// 2Dゲームフェーズに遷移する
	SetPhase(EGamePhase::EGP_2DGame);

	UDreamGameWidget* Widget = CreateWidget<UDreamGameWidget>(PlayerController, DreamGameWidgetClass);
	if (!Widget)
	{
		return;
	}

	// クリアイベントをバインドする
	Widget->OnDreamGameCompleted.AddUObject(this, &APlayerGameMode::HandleDreamGameCompleted);

	// ウィジェットをビューポートに追加する
	Widget->AddToViewport();
	ActiveDreamGameWidget = Widget;
}

void APlayerGameMode::StartIntroCutscene()
{
	if (IntroCutscene && CutsceneManager)
	{
		// 2Dイントロムービーのフェーズに遷移する
		SetPhase(EGamePhase::EGP_2DIntroMovie);

		// イントロムービーを再生する
		CutsceneManager->PlayCutscene(IntroCutscene);
	}
}

void APlayerGameMode::HandleDreamGameCompleted()
{
	if (ActiveDreamGameWidget)
	{
		// クリアイベントのバインドを解除する
		ActiveDreamGameWidget->OnDreamGameCompleted.RemoveAll(this);

		// ウィジェットをビューポートから削除する
		ActiveDreamGameWidget->RemoveFromParent();
		ActiveDreamGameWidget = nullptr;
	}

	// ----------------------------------------------------------
	// 2Dゲームクリア後、フェーズを切り替える（※現在は仮に3Dゲームフェーズに戻す）
	SetPhase(EGamePhase::EGP_3DGame);

	// 仮繋ぎ: Wakeムービー実装時にカットシーン経由へ差し替え
	if (AMyPlayerController* PC = GetMyPlayerController())
	{
		PC->SetViewTarget(PC->GetPawn());
	}
	// ----------------------------------------------------------
}

void APlayerGameMode::ShowResultWidget()
{
	SetPhase(EGamePhase::EGP_Result);

	if (ResultWidgetClass)
	{
		AMyPlayerController* PlayerController = GetMyPlayerController();
		if (PlayerController)
		{
			PlayerController->OpenMenuWidget(ResultWidgetClass);
		}
	}
}

void APlayerGameMode::SetPhaseWidgetInteraction(bool bEnabled)
{
	if (ActiveDreamGameWidget)
	{
		// ウィジェットの表示状態を切り替える
		ActiveDreamGameWidget->SetVisibility(
			bEnabled ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);

		// 復帰後、クリアボタンにフォーカスを当てる
		if (bEnabled)
		{
			ActiveDreamGameWidget->FocusCompletedButton();
		}
	}
}

void APlayerGameMode::SkipCurrentCutscene()
{
	if (CutsceneManager)
	{
		CutsceneManager->SkipCutscene();
	}
}

AMyPlayerController* APlayerGameMode::GetMyPlayerController() const
{
	return Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
}

void APlayerGameMode::HandleIntroCutsceneFinished()
{
	EnterDreamGame();
}
