#pragma once

#include "CoreMinimal.h"
#include "GamePhase.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	EGP_Title UMETA(DisplayName = "Title"),
	EGP_2DIntroMovie UMETA(DisplayName = "2DIntroMovie"),
	EGP_2DGame UMETA(DisplayName = "2DGame"),
	EGP_2DTo3DMovie UMETA(DisplayName = "2DTo3DMovie"),
	EGP_3DGame UMETA(DisplayName = "3DGame"),
	EGP_Result UMETA(DisplayName = "Result"),
};
