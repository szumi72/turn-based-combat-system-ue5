#include "QuestManager.h"
#include "MyRPGGameMode.h"


// Sets default values for this component's properties
UQuestManager::UQuestManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManager::ProgressActiveQuests()
{
	if (ActiveQuests.IsEmpty()) return;

	
	for (int32 i=ActiveQuests.Num()-1;i>=0;i--) {
		UQuest* Quest = ActiveQuests[i];
		if (Quest && !Quest->IsQuestCompleted() && Quest->AssignedUnit) {
			Quest->PerformStep();
			if (Quest->IsQuestCompleted()) {
				CompleteQuest(Quest);
				ActiveQuests.RemoveAt(i);
			}
		}
	}
}

void UQuestManager::CompleteQuest(UQuest* Quest)
{
	if (!Quest || !Quest->AssignedUnit) return;

	Quest->CompleteQuest();
	OnQuestCompleted.Broadcast(Quest->GetQuestAsset());

	FReward Reward = Quest->GiveReward();
	Quest->AssignedUnit->bIsOnQuest = false;
	Quest->AssignedUnit->SetActorHiddenInGame(false);
	Quest->AssignedUnit->SetActorEnableCollision(true);

	AMyRPGGameMode* GM = Cast<AMyRPGGameMode>(GetWorld()->GetAuthGameMode());	

	if (Reward.Type == ERewardType::Item && Reward.ItemAsset) {
		Quest->AssignedUnit->GetInventory()->AddItem(Reward.ItemAsset);		
		if (GM) {
			GM->AddLogBoardMes("Unit " + Quest->AssignedUnit->GetName() +  " has returned from quest and gain "+ Reward.ItemAsset->GetName());
		}
	}
	if (Reward.Type == ERewardType::Gold) {		
		if (GM) {
			GM->AddLogBoardMes("Unit " + Quest->AssignedUnit->GetName() + " has returned from quest and gain gold");
		}
	}
	
}

void UQuestManager::AddActiveQuest(UQuestAsset* Asset, ABaseRPGCharacter* Unit)
{
	if (!Unit || !Unit->IsAvailable())
	{
		return;
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
	NewQuest->InitFromAsset(Asset);
	NewQuest->StartQuest(Unit);
	ActiveQuests.Add(NewQuest);	

	AMyRPGGameMode* GM = Cast<AMyRPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->AddLogBoardMes("Unit " + Unit->GetName() + " has been sent on quest: " + Asset->GetName());
	}
	UE_LOG(LogRPG, Display, TEXT("[Quest Manager] %s was sent on a quest %s"),*Unit->GetName(), *Asset->GetName());
}