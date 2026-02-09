#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseRPGCharacter.h"
#include "SantaClaus.h"
#include "Elf.h"
#include "EngineUtils.h"
#include "MyPlayerController.h"
#include "BattleArena.h"
#include "QuestManager.h"



#include "MyRPGGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRPG, Log, All);

UCLASS()
class LABRPGGAMEUE_API AMyRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyRPGGameMode();
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool GetPlayerIsSanta() const;

	UFUNCTION(BlueprintCallable)
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentStage()const;

	UFUNCTION(BlueprintCallable)
	bool GetIsBattleWon()const;

	UFUNCTION(BlueprintCallable)
	UArmyComponent* GetSantaArmy()const { return SantaArmy; }

	UFUNCTION(BlueprintCallable)
	UArmyComponent* GetElfArmy()const { return ElfArmy; }

	// Starts the game after team selection (hides menu, shows HUD)
	UFUNCTION(BlueprintCallable)
	void StartGame();

	// Sets player faction (Santa or Elves) and starts the game
	UFUNCTION(BlueprintCallable)
	void SelectTeam(bool IsSanta);		

	// Recruits a new unit if the player has enough gold
	UFUNCTION(BlueprintCallable)
	void RecruitUnit();	

	// Starts a battle if both armies have units
	UFUNCTION(BlueprintCallable)
	void StartBattle();

	// Advances the game to the next stage and checks end game conditions
	UFUNCTION(BlueprintCallable)
	void NextStage();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBattleStarted();

	UPROPERTY(EditAnywhere, Category = "Battle")
	ABattleArena* BattleArena;

	// Called after QTE is resolved to execute player attack
	UFUNCTION(BlueprintCallable)
	void ResolvePlayerAttack(bool bQTESuccess);

	UFUNCTION(BlueprintCallable, Category = "Army")
	UArmyComponent* GetPlayerArmy() const;

	// Applies damage to the target during an attack animation
	UFUNCTION(BlueprintCallable, Category = "Animations")
	void OnPlayerDealDamage(ABaseRPGCharacter* Attacker, ABaseRPGCharacter* Target);

	UFUNCTION(BlueprintCallable, Category = "Animations")
	void OnPlayerAttackFinished(ABaseRPGCharacter* Attacker, ABaseRPGCharacter* Target);

	UFUNCTION(BlueprintCallable, Category = "Animations")
	void OnEnemyAttackFinished(ABaseRPGCharacter* Attacker, ABaseRPGCharacter* Target);	

	// Ends the battle and resolves win / loss
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void EndBattle();

	// Handles logic when a unit dies (cleanup, army update)
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void HandleUnitDeath(ABaseRPGCharacter* DeadUnit);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	ABaseRPGCharacter* GetCurrentPlayer();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	ABaseRPGCharacter* GetCurrentEnemy();

	UFUNCTION(BlueprintCallable)
	AMyPlayerController* GetMyPC();
	
	// Ends the current combat turn and resumes battle timer
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	// Adds a message to the on-screen log board
	UFUNCTION(BlueprintCallable, Category = "LogBoard")
	void AddLogBoardMes(const FString& Message);

	// Ends the game and shows win / lose screen
	UFUNCTION(BlueprintCallable)
	void EndGame(bool isWon);

	// Resets game state to initial values
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	bool GetGameResult()const { return GameResult; }


protected:

	UPROPERTY(VisibleInstanceOnly, Category = "Battle State")
	ABaseRPGCharacter* ActivePlayerUnit;

	UPROPERTY(VisibleInstanceOnly, Category = "Battle State")
	ABaseRPGCharacter* ActiveEnemyUnit;	

	FTimerHandle BattleTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "RPGLogic")
	int32 CurrentStage = 1;

	UPROPERTY(BlueprintReadOnly, Category = "RPGLogic")
	int32 Gold = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPGLogic")
	bool IsBattleWon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPGLogic")
	bool battleInProgress = false;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AMyPlayerController* GodControler;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABaseRPGCharacter> SantaBPClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABaseRPGCharacter> ElfBPClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quests")
	UQuestManager* QuestManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
	bool GameResult=false;
	
private:
	UPROPERTY()
	class UArmyComponent* SantaArmy;
	UPROPERTY()
	class UArmyComponent* ElfArmy;

	UPROPERTY(EditAnywhere)
	bool PlayerIsSanta;	

	UPROPERTY(EditAnywhere, Category = "RPGLogic")
	int32 UnitCost = 20;

	bool bWaitingForQTE = false;

	FVector LastPlayerCombatLoc;
	FVector LastEnemyCombatLoc;

	//spawning params
	FVector PlayerVisibleBaseLoc = FVector(800.0f, -50.0f, 92.0f);
	FVector EnemyHoldingArea = FVector(-2000.0f, 0.0f, -500.0f);
	float SpacingY = 120.0f;
	float SpacingX = 100.0f;
	int32 UnitsPerRow = 5;

	// Executes a single duel between two units
	void ExecuteDuel(ABaseRPGCharacter* Player, ABaseRPGCharacter* Enemy, bool bFullDamage);

	// Executes one battle turn (player + enemy actions)
	void ExecuteBattleTurn();

	// Spawns a unit for Santa or Elves
	void SpawnUnit(bool IsSanta);	

	// Spawns enemy army based on current stage
	void PrepareEnemyArmy();
	
	void BindPlayerDelegates(ABaseRPGCharacter* Player);

	// Prepares player attack logic before execution
	void PreparePlayerAttack(ABaseRPGCharacter* Player, bool bFullDamage);
	FVector CalculateSpwanPosition(bool bIsPlayerUnit);
	void SelectActiveUnits();
	void CacheCombatLocations();

	// Starts battle camera and presentation
	void StartBatllePresentation();
	void StartPlayerQTE();

	// Checks if battle can start (both armies have units)
	bool IsBattlePssible();
	
};


