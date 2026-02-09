// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


#include "BaseRPGCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageEvent, ABaseRPGCharacter*, Attacker, ABaseRPGCharacter*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackFinishedEvent, ABaseRPGCharacter*, Attacker, ABaseRPGCharacter*, Victim);



UENUM(BlueprintType)
enum class ECharacterAnimState : uint8
{
	Idle,
	Attack,
	Hit,
	Death
};

UCLASS()
class LABRPGGAMEUE_API ABaseRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseRPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, Category = "Getter")
	FString GetName()const;
	UFUNCTION(BlueprintCallable, Category = "Getter")
	int32 GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Getter")
	int32 GetBaseDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int32 GetDamage() const { return Damage; };

	UFUNCTION(BlueprintCallable)
	void SetCustomName(FString NewName) { Name = NewName; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	UInventoryComponent* GetInventory() const;

	UFUNCTION(BlueprintCallable, Category = "Getter")
	ECharacterAnimState GetAnimState()const { return AnimState; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	ABaseRPGCharacter* GetCurrentTarget()const { return CurrentTarget; }

	//Handles character death, enables ragdol and play death animation
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleDeath(AActor* DeadActor);

	//adds all equipments damage bonuses to character damage 
	UFUNCTION(BlueprintCallable, Category = "Combat")
	int32 ComputeFullDamage();
	
	//reduces character CurrentHealth by RecivedDamage
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ReceiveDamage(int32 RecivedDamage);

	UFUNCTION(BlueprintCallable)
	bool IsAlive()const;

	UFUNCTION(BlueprintCallable, Category = "Anim")
	void PlayAttack();

	UFUNCTION(BlueprintCallable, Category = "Anim")
	void PlayHit();

	UFUNCTION(BlueprintCallable, Category = "Anim")
	void PlayDeath();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnableRagdoll();
	
	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAttacking() const { return bIsAttacking; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsOnQuest = false;

	bool IsAvailable() const { return (IsAlive() && !bIsOnQuest); }

	UPROPERTY(BlueprintAssignable)
	FOnDamageEvent OnDamageEvent;

	UPROPERTY(BlueprintAssignable)
	FOnAttackFinishedEvent OnAttackFinishedEvent;	

	//sets CurrentTarget as Target and play AttackMontage
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackTarget(ABaseRPGCharacter* Target, int32 DamageToDeal);
	
	UFUNCTION(BlueprintCallable, Category = "Notify")
	void AnimNotify_Hit();

	UFUNCTION(BlueprintCallable, Category = "Notify")
	void AnimNotify_OnAnimationFinished();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	int32 RollDamage(int32 dice = 8);

	//divides character damage by 3
	UFUNCTION(BlueprintCallable, Category = "Combat")
	int32 ReduceDamage();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UHealthComponent* Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	int32 BaseDamage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	ECharacterAnimState AnimState = ECharacterAnimState::Idle;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	class UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly)
	ABaseRPGCharacter* CurrentTarget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 Damage = 0;


};
