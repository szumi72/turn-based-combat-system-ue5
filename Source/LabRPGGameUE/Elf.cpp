
#include "Elf.h"

AElf::AElf()
{
	Name = "Elfik";
	BaseDamage = 15;	
}

void AElf::BeginPlay()
{
	Super::BeginPlay();
	Health->ModifyHealth(50);	
	UE_LOG(LogTemp, Display, TEXT("[Elf] %s was created as Elf, HP: %d, BaseDamage: %d"), *Name,Health->GetCurrentHealth(),GetBaseDamage());
}
