#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Reward.h"
#include "QuestAsset.generated.h"


UCLASS(BlueprintType)
class LABRPGGAMEUE_API UQuestAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestName = "Undefined";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 Duration;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FReward Reward;

};
