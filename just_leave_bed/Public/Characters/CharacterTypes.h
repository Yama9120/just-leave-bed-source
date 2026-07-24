#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.generated.h"

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
};
