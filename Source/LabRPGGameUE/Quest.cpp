#include "Quest.h"

void UQuest::PerformStep()
{
	if (IsQuestCompleted()) return;

	Progress++;
	if (Progress >= Quest->Duration) {
		CompleteQuest();
	}

}

void UQuest::StartQuest(ABaseRPGCharacter* Unit)
{
	if (!Unit) return;

	AssignedUnit = Unit;
	AssignedUnit->bIsOnQuest = true;	
	IsCompleted = false;
	AssignedUnit->SetActorHiddenInGame(true);
	AssignedUnit->SetActorEnableCollision(false);
}

void UQuest::CompleteQuest()
{
	IsCompleted = true;

}

void UQuest::InitFromAsset(UQuestAsset* Asset)
{
	Quest = Asset;
	IsCompleted = false;
	InProgress = true;
}
