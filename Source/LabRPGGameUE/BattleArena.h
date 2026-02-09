// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "BattleArena.generated.h"

class ABaseRPGCharacter;


UCLASS()
class LABRPGGAMEUE_API ABattleArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleArena();
public:	
	UFUNCTION(BlueprintCallable)
	void StartBattle(ABaseRPGCharacter* Player, ABaseRPGCharacter* Enemy);
	
	UFUNCTION(BlueprintCallable)
	void ActivateBattleCamera(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void DeactivateBattleCamera(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetBattleCamera() const { return BattleCamera; }

	UFUNCTION()
	virtual void CalcCamera(float DeltaTime,FMinimalViewInfo& OutResult) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Arena", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Arena", meta = (AllowPrivateAccess = "true"))
	USceneComponent* PlayerSlot;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Arena", meta = (AllowPrivateAccess = "true"))
	USceneComponent* EnemySlot;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Arena", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* BattleCamera;

};
