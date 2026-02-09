// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRPGCharacter.h"
#include "MyRPGGameMode.h"


// Sets default values
ABaseRPGCharacter::ABaseRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

}

// Called when the game starts or when spawned
void ABaseRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Health)
	{
		Health->OnDeath.AddDynamic(this, &ABaseRPGCharacter::HandleDeath);
	}	
}

// Called to bind functionality to input
void ABaseRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FString ABaseRPGCharacter::GetName() const
{
	return Name;
}

int32 ABaseRPGCharacter::GetHealth() const
{
	return Health->GetCurrentHealth();
}

int32 ABaseRPGCharacter::GetBaseDamage() const
{
	return BaseDamage;
}

UInventoryComponent* ABaseRPGCharacter::GetInventory() const
{
	return Inventory;
		
}

void ABaseRPGCharacter::HandleDeath(AActor* DeadActor)
{
	
	if (IsAlive()) return;	
	if (bIsDead) return;
	bIsDead = true;
	PlayDeath();
	UE_LOG(LogRPG, Display, TEXT("[Character] %s is dead"), *Name);
	EnableRagdoll();	

}


int32 ABaseRPGCharacter::ComputeFullDamage()
{
	int32 TotalDamage = GetBaseDamage();
	if (IsValid(Inventory)) {
		for (UItem* It : Inventory->GetItems()) {
			TotalDamage+=It->Damage;
		}
	}
	if (TotalDamage > Damage) {
		Damage = TotalDamage;
	}
	return Damage;
}

void ABaseRPGCharacter::ReceiveDamage(int32 RecivedDamage)
{
	Health->ModifyHealth(-RecivedDamage);
}

bool ABaseRPGCharacter::IsAlive() const
{
	return Health->IsAlive();
}

void ABaseRPGCharacter::PlayAttack()
{
	if (!IsAlive()) return;
	AnimState = ECharacterAnimState::Attack;
}

void ABaseRPGCharacter::PlayHit()
{
	if (!IsAlive()) return;
	AnimState = ECharacterAnimState::Hit;
}

void ABaseRPGCharacter::PlayDeath()
{
	AnimState = ECharacterAnimState::Death;
}

void ABaseRPGCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Radgedoll"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);	
	SetLifeSpan(3.0f);	
}

float ABaseRPGCharacter::GetHealthPercent() const
{
	return (GetHealth() / float(Health->GetMaxHealth()));
}

void ABaseRPGCharacter::AttackTarget(ABaseRPGCharacter* Target, int32 DamageToDeal)
{
	if (!IsAlive() || !Target) return;
	
	CurrentTarget = Target;
	Damage = DamageToDeal;	
	bIsAttacking = true;
	
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);		
		FVector LookAt = Target->GetActorLocation() - GetActorLocation();
		LookAt.Z = 0.f;
		SetActorRotation(LookAt.Rotation());
		
	}
}

void ABaseRPGCharacter::AnimNotify_Hit()
{
	if (!CurrentTarget || !IsAlive())return;	
		
	UE_LOG(LogRPG, Display, TEXT("[Character] %s hits %s for %d dmg!"), *GetName(), *CurrentTarget->GetName(), Damage);
	AMyRPGGameMode* GM = Cast<AMyRPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->AddLogBoardMes(GetName() + " hits " + CurrentTarget->GetName()+ " for " + FString::FromInt(Damage));
	}
	OnDamageEvent.Broadcast(this,CurrentTarget);	
}

void ABaseRPGCharacter::AnimNotify_OnAnimationFinished()
{
	OnAttackFinishedEvent.Broadcast(this,CurrentTarget);
}

int32 ABaseRPGCharacter::RollDamage(int32 AttackDice)
{
	int32 toHit = 1 + FMath::Rand() % 20;

	int32 attack = 1 + FMath::Rand() % AttackDice;
	Damage = ComputeFullDamage() + toHit + attack;
	
	if (toHit == 20) {
		UE_LOG(LogRPG, Display, TEXT("[Character] %s rolled Natural 20! Damage is doubled."), * Name);
		Damage *= 2;
	}
	return Damage;
}

int32 ABaseRPGCharacter::ReduceDamage()
{
	Damage /= 3;
	return Damage;
}


