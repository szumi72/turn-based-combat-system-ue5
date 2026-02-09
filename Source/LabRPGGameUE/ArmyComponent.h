// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseRPGCharacter.h"
#include "ArmyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LABRPGGAMEUE_API UArmyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UArmyComponent();
	UFUNCTION(BlueprintCallable, Category = "Army")
	bool HasUnits();
	
	UFUNCTION(BlueprintCallable, Category = "Army")
	ABaseRPGCharacter* GetActiveUnit();

	UFUNCTION(BlueprintCallable, Category = "Army")
	void AddUnit(ABaseRPGCharacter* NewUnit);

	UFUNCTION(BlueprintCallable, Category = "Army")
	void SetName(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Army")
	FString GetName()const;

	//Destroys all units in the Army
	UFUNCTION(BlueprintCallable, Category = "Army")
	void ClearArmy();	

	UFUNCTION(BlueprintCallable, Category = "Army")
	TArray<ABaseRPGCharacter*> GetUnits();
	
	//Destroys all dead units in the Army
	UFUNCTION(BlueprintCallable, Category = "Army")
	void CleanupDeadUnits();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Army")
	TArray<ABaseRPGCharacter*>Army;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Army")
	int32 UnitsCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Army")
	FString ArmyName;

	int32 static nameCounter;
		
};
