#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "QuestAsset.h"
#include "QuestManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted,UQuestAsset*, QuestAsset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LABRPGGAMEUE_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestManager();

	//performs step of every active quest
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ProgressActiveQuests();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteQuest(UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddActiveQuest(UQuestAsset* Asset, ABaseRPGCharacter* Unit);

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestCompleted OnQuestCompleted;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<UQuestAsset*> AvaliableQuestsAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<UQuest*> ActiveQuests;

		
};
