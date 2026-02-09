#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "Item.h"
#include "Reward.generated.h"


UENUM(BlueprintType)
enum class ERewardType : uint8
{
	Gold,
	Item
};

USTRUCT(BlueprintType)
struct FReward
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	ERewardType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 GoldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	UItem* ItemAsset;
	
};
