#pragma once

#include "CoreMinimal.h"
#include "BaseRPGCharacter.h"
#include "SantaClaus.generated.h"

UCLASS()
class LABRPGGAMEUE_API ASantaClaus : public ABaseRPGCharacter
{
	GENERATED_BODY()
public:	
	ASantaClaus();
	void BeginPlay() override;
	
};
