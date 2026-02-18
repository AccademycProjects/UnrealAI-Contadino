
#pragma once

#include "CoreMinimal.h"
#include "EQSTagMatchType.generated.h"

UENUM(BlueprintType)
enum class EEQSTagMatchType : uint8
{
	Any     UMETA(DisplayName = "Match Any"),
	All     UMETA(DisplayName = "Match All"),
	Exact   UMETA(DisplayName = "Exact Match")
};
