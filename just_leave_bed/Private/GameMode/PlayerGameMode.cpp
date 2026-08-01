// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/PlayerGameMode.h"
#include "UI/Menu/StartWidget.h"
#include "UI/Menu/ResultWidget.h"
#include "UI/DreamGame/DreamGameWidget.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/MyPlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"

void APlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (StartWidgetClass)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OpenMenuWidget(StartWidgetClass);
		}
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
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
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
	// 遷移元のフェーズを確認する（※現在は仮にタイトルからの遷移を想定）
	if (CurrentPhase != EGamePhase::EGP_Title)
	{
		return;
	}

	if (!DreamGameWidgetClass)
	{
		return;
	}

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// 3Dキャラ操作を外し、2Dゲーム用マッピングに入れ替える
		if (const APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerController->GetPawn()))
		{
			Subsystem->RemoveMappingContext(Character->GetPlayer3DContext());
		}
		Subsystem->AddMappingContext(Player2DContext, 0);
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

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 2Dゲーム用マッピングを外し、3Dキャラ操作用マッピングに戻す
			Subsystem->RemoveMappingContext(Player2DContext);

			// 決定を押した後、次のフェーズで同じキーの入力をすぐには受け付けないように設定
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = true; // 切替後ジャンプ等の暴発を防ぐため、すべての押下中のキーが離されるまで無視する
			if (const APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerController->GetPawn()))
			{
				Subsystem->AddMappingContext(Character->GetPlayer3DContext(), 0, Options);
			}
		}
	}

	// 2Dゲームクリア後、フェーズを切り替える（※現在は仮に3Dゲームフェーズに戻す）
	SetPhase(EGamePhase::EGP_3DGame);
}

void APlayerGameMode::ShowResultWidget()
{
	SetPhase(EGamePhase::EGP_Result);

	if (ResultWidgetClass)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
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
