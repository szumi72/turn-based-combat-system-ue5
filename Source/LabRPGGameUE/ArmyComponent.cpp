// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmyComponent.h"

int32 UArmyComponent::nameCounter = 0;


UArmyComponent::UArmyComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;	
}

bool UArmyComponent::HasUnits()
{
	if (Army.IsEmpty()) return false;
	
	for (auto& Unit : Army) {
		if (Unit && Unit->IsAvailable()) {
			return true;
		}
	}	
	return false;
}

ABaseRPGCharacter* UArmyComponent::GetActiveUnit()
{	
	if (!HasUnits()) return nullptr;
	for (auto& Unit : Army) {
		if (Unit && Unit->IsAvailable()) {
			return Unit;
		}
	}	
	return nullptr;
}

void UArmyComponent::AddUnit(ABaseRPGCharacter* NewUnit)
{
	if (NewUnit) {
		Army.Add(NewUnit);
		NewUnit->bIsOnQuest = false;
		UnitsCount = Army.Num();
		FString UniqueName = NewUnit->GetName() + FString::Printf(TEXT("_%d"), nameCounter);
		NewUnit->SetCustomName(UniqueName);
		nameCounter++;
		UE_LOG(LogTemp, Display, TEXT("[Army] New unit added to %s, current count: %d"),*ArmyName ,UnitsCount);
	}
}

void UArmyComponent::SetName(FString Name)
{
	ArmyName = Name;
}

FString UArmyComponent::GetName() const
{
	return ArmyName;
}

void UArmyComponent::ClearArmy()
{
	for (ABaseRPGCharacter* Unit : Army) {
		if (IsValid(Unit)) {
			Unit->Destroy();
		}
	}
	Army.Empty();	
}

TArray<ABaseRPGCharacter*> UArmyComponent::GetUnits()
{
	return Army;
}

void UArmyComponent::CleanupDeadUnits()
{
	for (int32 i = Army.Num() - 1; i >= 0; i--)
	{
		if (!Army[i] || !Army[i]->IsAlive())
		{
			Army.RemoveAt(i);
			UE_LOG(LogTemp, Display, TEXT("[Army] Cleanup army services [%s] current army count: %d"), *GetName(), Army.Num());
			UnitsCount--;
		}
	}
	
}



