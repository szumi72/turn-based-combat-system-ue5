
#include "InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UItem * StartItem : DefaultItems)
	{
		if (StartItem)
		{
			Items.Add(StartItem);
		}
	}
}
void UInventoryComponent::AddItem(UItem* item)
{
	if (item) {
		Items.Add(item);		
	}	
}





