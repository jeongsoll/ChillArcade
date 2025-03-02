﻿#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.generated.h"

#define MAP_ROW_MAX 15
#define MAP_COLUMN_MAX 17
#define MAP_MIN -1

#define PLAYER_INITIAL_SPEED 3
#define PLAYER_SIZE 80
#define PLAYER_MAX_SPEED 7
#define MAX_BALLOON_COUNT 6
#define MAX_BALLOON_RANGE 6

#define SLOW_RIDE_SPEED 1
#define FAST_RIDE_SPEED 6

#define ITEM_COUNT 20


USTRUCT(BlueprintType)
struct FArrLocation {
	GENERATED_BODY()

	UPROPERTY()
	int32 X{};
	UPROPERTY()
	int32 Y{};

	bool operator==(const FArrLocation& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

UENUM(BlueprintType)
enum EMapType : uint8 {
	Movable = 0,
	Blocking = 1,
	Destroyable = 2,
	Pushable = 3,
	Bush = 4,
	BalloonLoc = 10,

	BubbleItem = 20,
	CanItem = 21,
	DevilItem = 22,
	FluidItem = 23,
	NeedleItem = 24,
	RangeItem = 25,
	RollerItem = 26,
	ShieldItem = 27,
	SpaceShipItem = 28,
	SpannerItem = 29,
	TurtleItem = 30,
	// ~ 40 까지 사용 금지
	PlayerLoc = 100,
	PlayerBalloonLoc = 110
};

UENUM(BlueprintType)
enum EItemType : uint8 {
	None = 0,
	Can = 1,
	Needle = 2,
	Shield = 3,
	Spanner = 4,
	Five = 5,
	Six = 6
};

//타일 유형
UENUM(BlueprintType)
enum class ETileType : uint8
{
	Movable = 0x00, // 0000 0000
	Destroyable = 0x01, // 0000 0001
	Blocking = 0x02, // 0000 0010
	/*Pushable = 0x03, // 0000 0011
	Bush = 0x04 // 0000 0100*/
};
//오브젝트 유형
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	OBJ_None = 0x00, // 0000 0000
	OBJ_Bomb = 0x10, // 0001 0000
	OBJ_Character = 0x20, // 0010 0000
	OBJ_Ai = 0x30, // 0100 0000
	OBJ_Item = 0x40 // 1000 0000
};

USTRUCT(BlueprintType)
struct FNode_Info
{
	GENERATED_BODY()
	
	FVector2D Position;
	int GCost; // 이동비용
	int HCost; // 목표까지 예상 거리
	FNode_Info* Parent; // 부모 노드
	FNode_Info() : Position(FVector2D(0, 0)), GCost(0), HCost(0), Parent(nullptr){};

	int GetCostF() const {return GCost + HCost;}
	int GetCostH() const {return HCost;}
	FVector2D GetCurBlock() const {return Position;}
	FNode_Info* GetParent() const {return Parent;}
	FVector2D SetCurBlock(FVector2D NewPosition) {return Position = NewPosition;}
};

inline int8 map[15][17] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 1,	// 5
	1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 0, 0, 1,	// 2
	1, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,	// 3
	1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1,	// 5
	1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,	// 5
	1, 2, 1, 2, 1, 2, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 1,	// 8
	1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,	// 0
	1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1, 2, 1, 2, 1, 2, 1,	// 8
	1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 1,	// 5
	1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,	// 5
	1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 1,	// 4
	1, 0, 0, 2, 2, 2, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,	// 3
	1, 1, 0, 1, 2, 1, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1,	// 4
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};


/*inline int8 map[15][17] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 2, 2, 4, 0, 0, 3, 4, 1, 2, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 3, 1, 3, 0, 0, 1, 2, 2, 2, 2, 2, 1,
	1, 0, 0, 2, 2, 2, 4, 0, 3, 3, 4, 1, 3, 2, 2, 2, 1,
	1, 3, 1, 3, 1, 3, 1, 3, 0, 0, 1, 2, 2, 2, 2, 2, 1,
	1, 2, 2, 2, 2, 2, 4, 0, 0, 3, 4, 1, 3, 1, 3, 1, 1,
	1, 2, 1, 2, 1, 2, 1, 3, 3, 0, 0, 2, 2, 2, 2, 2, 1,
	1, 1, 4, 1, 4, 1, 4, 0, 0, 3, 4, 1, 4, 1, 4, 1, 1,
	1, 2, 2, 2, 2, 2, 0, 3, 0, 0, 1, 2, 1, 2, 1, 2, 1,
	1, 1, 3, 1, 3, 1, 4, 0, 3, 3, 4, 2, 2, 2, 2, 2, 1,
	1, 2, 2, 2, 2, 2, 1, 3, 0, 0, 1, 3, 1, 3, 1, 3, 1,
	1, 0, 0, 1, 3, 1, 4, 0, 0, 3, 4, 2, 2, 3, 3, 3, 1,
	1, 0, 0, 2, 2, 2, 1, 3, 3, 0, 1, 3, 1, 3, 3, 3, 1,
	1, 0, 0, 0, 2, 1, 4, 0, 0, 3, 4, 2, 2, 3, 3, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};*/