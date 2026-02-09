#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
}

bool UHealthComponent::IsAlive() const
{
	return CurrentHealth > 0;
}

void UHealthComponent::TakeDamage(int32 Damage)
{
	if (IsAlive()) {

		CurrentHealth = CurrentHealth > Damage ? CurrentHealth - Damage : 0;
	}
	if (CurrentHealth <= 0) {
		OnDeath.Broadcast(GetOwner());
	}
}

int32 UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UHealthComponent::ModifyHealth(int32 InHealth)
{
	CurrentHealth = CurrentHealth + InHealth;
	if(CurrentHealth<=0){
		OnDeath.Broadcast(GetOwner());
	}
}



