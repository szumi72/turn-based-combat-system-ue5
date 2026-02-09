// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleArena.h"
#include "BaseRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABattleArena::ABattleArena()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	PlayerSlot = CreateDefaultSubobject<USceneComponent>(TEXT("Player Slot"));
	EnemySlot = CreateDefaultSubobject<USceneComponent>(TEXT("Enemy Slot"));
	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));

	PlayerSlot->SetupAttachment(Root);
	EnemySlot->SetupAttachment(Root);
	BattleCamera->SetupAttachment(Root);

}

void ABattleArena::StartBattle(ABaseRPGCharacter* Player, ABaseRPGCharacter* Enemy)
{
	if (!Player || !Enemy) return;

	Player->SetActorEnableCollision(false);
	Enemy->SetActorEnableCollision(false);
		
	Player->TeleportTo(	PlayerSlot->GetComponentLocation(), PlayerSlot->GetComponentRotation());
	Enemy->TeleportTo(EnemySlot->GetComponentLocation(),EnemySlot->GetComponentRotation());

	Player->GetCharacterMovement()->DisableMovement();
	Enemy->GetCharacterMovement()->DisableMovement();

}

void ABattleArena::ActivateBattleCamera(APlayerController* PC)
{	
	if (!PC) return;

	PC->SetViewTargetWithBlend(this,0.5f,EViewTargetBlendFunction::VTBlend_Cubic,0.0f,true);
}

void ABattleArena::DeactivateBattleCamera(APlayerController* PC)
{
	if (!PC) return;

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn) return;

	PC->SetViewTargetWithBlend(PlayerPawn,0.5f,EViewTargetBlendFunction::VTBlend_Cubic,0.0f,true);
}

void ABattleArena::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	if (BattleCamera)
	{
		BattleCamera->GetCameraView(DeltaTime, OutResult);
	}
}




