// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LABRPGGAMEUE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

	// Sets default values for this component's properties
	UHealthComponent();
	UFUNCTION(BlueprintCallable, Category = "Health")
	//returns true if CurrentHealth is above zero
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(int32 Damage);

	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetCurrentHealth()const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetMaxHealth()const { return MaxHealth; }

	//adds InHealth to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ModifyHealth(int32 InHealth);
	//callesd whenever CurrentHealth drops to zero
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
protected:	
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	int32 CurrentHealth = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	int32 MaxHealth = 100;		
};
