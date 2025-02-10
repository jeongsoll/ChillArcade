#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.generated.h"

#define MAP_ROW_MAX 17
#define MAP_COLUMN_MAX 15
#define MAP_MIN -1

USTRUCT(BlueprintType)
struct FArrLocation {
	GENERATED_BODY()

	UPROPERTY()
	int32 X{};
	UPROPERTY()
	int32 Y{};
};
