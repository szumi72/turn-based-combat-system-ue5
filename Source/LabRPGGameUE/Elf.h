#pragma once

#include "CoreMinimal.h"
#include "BaseRPGCharacter.h"
#include "Elf.generated.h"

UCLASS()
class LABRPGGAMEUE_API AElf : public ABaseRPGCharacter
{
	GENERATED_BODY()
public:
	AElf();
	void BeginPlay() override;
};
