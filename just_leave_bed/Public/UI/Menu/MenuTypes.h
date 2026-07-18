#pragma once

#include "CoreMinimal.h"
#include "MenuTypes.generated.h"

// 入力モードの種類を列挙型で定義
UENUM(BlueprintType)
enum class EMenuInputMode : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI
};
