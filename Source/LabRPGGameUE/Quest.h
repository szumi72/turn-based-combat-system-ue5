#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseRPGCharacter.h"
#include "QuestAsset.h"

#include "Quest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LABRPGGAMEUE_API UQuest : public UObject
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	FString GetQuestName()const { return Quest->QuestName; }

	UFUNCTION(BlueprintCallable)
	UQuestAsset* GetQuestAsset()const { return Quest; }

	UFUNCTION(BlueprintCallable)
	bool IsQuestCompleted()const { return IsCompleted; }

	UFUNCTION(BlueprintCallable)
	bool GetQuestInProgress()const { return InProgress; }

	UFUNCTION(BlueprintCallable)
	void PerformStep();

	UFUNCTION(BlueprintCallable)
	FReward GiveReward()const { return Quest->Reward; }	

	UFUNCTION(BlueprintCallable)
	void StartQuest(ABaseRPGCharacter* Unit);

	UFUNCTION(BlueprintCallable)
	void CompleteQuest();

	UFUNCTION(BlueprintCallable)
	void InitFromAsset(UQuestAsset* Asset);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
	ABaseRPGCharacter* AssignedUnit = nullptr;

protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 Progress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UQuestAsset* Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool InProgress = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool IsCompleted = false;
	
	
};
