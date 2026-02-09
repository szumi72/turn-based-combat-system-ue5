#include "SantaClaus.h"

ASantaClaus::ASantaClaus()
{
	Name = "Santa";
	BaseDamage = 10;
}

void ASantaClaus::BeginPlay()
{
	Super::BeginPlay();
	Health->ModifyHealth(55);	
	UE_LOG(LogTemp, Display, TEXT("[Santa] %s was created as SantaClaus HP: %d, BaseDamage: %d"), *Name, Health->GetCurrentHealth(), GetBaseDamage());
}
