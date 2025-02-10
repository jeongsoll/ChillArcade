#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.generated.h"

USTRUCT(BlueprintType)
struct FArrLocation {
	GENERATED_BODY()

	UPROPERTY()
	int32 X{};
	UPROPERTY()
	int32 Y{};
};
