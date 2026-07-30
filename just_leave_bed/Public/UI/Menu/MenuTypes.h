#pragma once

#include "CoreMinimal.h"
#include "MenuTypes.generated.h"

// 入力モードの種類を列挙型で定義
UENUM(BlueprintType)
enum class EMenuInputMode : uint8
{
	EMIM_GameOnly UMETA(DisplayName = "GameOnly"),
	EMIM_UIOnly UMETA(DisplayName = "UIOnly"),
	EMIM_GameAndUI UMETA(DisplayName = "GameAndUI"),
};

USTRUCT(BlueprintType)
struct FInputProfile
{
	GENERATED_BODY()

	// 入力モード（Game/UI/両方）
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMenuInputMode InputMode = EMenuInputMode::EMIM_GameOnly;

	// 移動入力を無視するか
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIgnoreMoveInput = false;

	// 視点入力を無視するか
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIgnoreLookInput = false;

	// マウスカーソルを表示するか
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowCursor = false;
};
