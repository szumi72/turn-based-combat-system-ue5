#include "MyRPGGameMode.h"
#include "ArmyComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogRPG);

AMyRPGGameMode::AMyRPGGameMode()
{
	DefaultPawnClass = ABaseRPGCharacter::StaticClass();
	SantaArmy = CreateDefaultSubobject<UArmyComponent>(TEXT("SantaArmyComponent"));
	SantaArmy->SetName("SantaArmy");
	ElfArmy = CreateDefaultSubobject<UArmyComponent>(TEXT("ElfArmyComponent"));
	ElfArmy->SetName("ElfArmy");
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("QuestManagerComponent"));
}

void AMyRPGGameMode::BeginPlay()
{	
	Super::BeginPlay();	
	GodControler = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GodControler) {
		GodControler->ShowMenu();
	}
	TArray<AActor*>Arenas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleArena::StaticClass(), Arenas);
	if (Arenas.Num() > 0) {
		BattleArena = Cast<ABattleArena>(Arenas[0]);
	}	
}

void AMyRPGGameMode::StartGame()
{
	UE_LOG(LogRPG, Display, TEXT("[GameMode] Game Started"));
	if (GodControler) {
		GodControler->HideMenu();
		GodControler->ShowHUD();
	}
}

bool AMyRPGGameMode::GetPlayerIsSanta() const
{
	return PlayerIsSanta;
}

int32 AMyRPGGameMode::GetGold() const
{
	return Gold;
}

int32 AMyRPGGameMode::GetCurrentStage() const
{
	return CurrentStage;
}

bool AMyRPGGameMode::GetIsBattleWon()const
{
	return IsBattleWon;
}

void AMyRPGGameMode::SelectTeam(bool IsSanta)
{
	PlayerIsSanta = IsSanta;
	if (PlayerIsSanta) {
		UE_LOG(LogRPG, Display, TEXT("[GameMode] Player joined Santa's Team"));
		AddLogBoardMes("You joined Santa's Team!");
	}
	else {
		UE_LOG(LogRPG, Display, TEXT("[GameMode] Player joined Elf's Team"));
		AddLogBoardMes("You joined Elf's Team!");
	}
	StartGame();
}

void AMyRPGGameMode::ExecuteDuel(ABaseRPGCharacter* Player, ABaseRPGCharacter* Enemy, bool bFullDamage)
{
	if (IsValid(Player) && IsValid(Enemy)) {
		
		BindPlayerDelegates(Player);		
		PreparePlayerAttack(Player, bFullDamage);
		Player->AttackTarget(Enemy, Player->GetDamage());
		
	}
}

void AMyRPGGameMode::BindPlayerDelegates(ABaseRPGCharacter* Player) 
{
	Player->OnAttackFinishedEvent.AddUniqueDynamic(this, &AMyRPGGameMode::OnPlayerAttackFinished);
	Player->OnDamageEvent.AddUniqueDynamic(this, &AMyRPGGameMode::OnPlayerDealDamage);
}

void AMyRPGGameMode::PreparePlayerAttack(ABaseRPGCharacter* Player, bool bFullDamage)
{
	Player->RollDamage(8);
	if (!bFullDamage) {
		Player->ReduceDamage();
		UE_LOG(LogRPG, Display, TEXT("[GameMode] QTE failed Damage was devided by 3, Damage: %d"), Player->GetDamage());
		AddLogBoardMes("Weak Attack! Damage reduced");	}
	else {
		UE_LOG(LogRPG, Display, TEXT("[GameMode] QTE succeded, Damage: %d"), Player->GetDamage());
	}
}	

void AMyRPGGameMode::ExecuteBattleTurn(){	

	if (bWaitingForQTE || battleInProgress)
	{
		return; 
	}		
	
	SelectActiveUnits();	
	if (!ActivePlayerUnit || !ActiveEnemyUnit) {	
		EndBattle();
		return;
	}	

	CacheCombatLocations();
	StartBatllePresentation();
	StartPlayerQTE();	
}

void AMyRPGGameMode::SelectActiveUnits() 
{
	ActivePlayerUnit = GetCurrentPlayer();
	ActiveEnemyUnit = GetCurrentEnemy();
}

void AMyRPGGameMode::CacheCombatLocations() 
{
	LastPlayerCombatLoc = ActivePlayerUnit->GetActorLocation();
	LastEnemyCombatLoc = ActiveEnemyUnit->GetActorLocation();
}

void AMyRPGGameMode::StartBatllePresentation()
{
	battleInProgress = true;
	bWaitingForQTE = true;

	if (BattleArena)
	{
		BattleArena->StartBattle(ActivePlayerUnit, ActiveEnemyUnit);
	}
}

void AMyRPGGameMode::StartPlayerQTE() 
{
	if (AMyPlayerController* PC = GetMyPC()) {

		PC->StartQTE();
	}
	GetWorldTimerManager().PauseTimer(BattleTimerHandle);
}

void AMyRPGGameMode::SpawnUnit(bool IsSanta)
{		
	bool bIsPlayerUnit = (IsSanta == PlayerIsSanta);	
	FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
	FVector SpawnLocation = CalculateSpwanPosition(bIsPlayerUnit);
	
	if (IsSanta && SantaBPClass)
	{
		ASantaClaus* Santa = GetWorld()->SpawnActor<ASantaClaus>(SantaBPClass, SpawnLocation, SpawnRotation);
		SantaArmy->AddUnit(Santa);
		UE_LOG(LogRPG, Display, TEXT("[GameMode] Spawned Santa for %s team"), bIsPlayerUnit ? TEXT("Player") : TEXT("Enemy"));
	}
	else if (!IsSanta && ElfBPClass)
	{
		AElf* Elf = GetWorld()->SpawnActor<AElf>(ElfBPClass, SpawnLocation, SpawnRotation);
		ElfArmy->AddUnit(Elf);
		UE_LOG(LogTemp, Warning, TEXT("[GameMode] Spawned Elf for %s team"), bIsPlayerUnit ? TEXT("Player") : TEXT("Enemy"));
	}
}

FVector AMyRPGGameMode::CalculateSpwanPosition(bool bIsPlayerUnit)
{
	FVector SpawnLocation;

	if (bIsPlayerUnit)
	{
		UArmyComponent* CurrentArmy = GetPlayerArmy();
		int32 CurrentCount = CurrentArmy->GetUnits().Num();

		int32 Row = CurrentCount / UnitsPerRow;
		int32 Col = CurrentCount % UnitsPerRow;

		float RowWidth = (UnitsPerRow - 1) * SpacingY;
		float CenterOffset = RowWidth / 2.0f;

		float OffsetX = Row * SpacingX;
		float OffsetY = (Col * SpacingY) - CenterOffset;

		return (SpawnLocation = PlayerVisibleBaseLoc + FVector(OffsetX, OffsetY, 0.0f));
	}
	else
	{
		float RandomOffset = FMath::RandRange(-200.0f, 200.0f);
		return (SpawnLocation = EnemyHoldingArea + FVector(RandomOffset, RandomOffset, 0.0f));
		
	}
}

void AMyRPGGameMode::RecruitUnit()
{
	UE_LOG(LogRPG, Display, TEXT("[GameMode] Current Gold: %d, Cost: %d"), Gold, UnitCost);
	if (Gold >= UnitCost) {
		Gold -= UnitCost;
		SpawnUnit(PlayerIsSanta);
		UE_LOG(LogRPG, Display, TEXT("[GameMode] Gold left: %d"),Gold);
	}
	else {
		UE_LOG(LogRPG, Display, TEXT("[GameMode] Not enough money"));
		AddLogBoardMes("Not enough money for recruitment");
	}
}

void AMyRPGGameMode::PrepareEnemyArmy()
{
	UE_LOG(LogRPG, Display, TEXT("[GameMode] Preparing enemy team"));	
	for (int i = 0; i < 2 * CurrentStage; i++) {
		SpawnUnit(!PlayerIsSanta);
	}
}

void AMyRPGGameMode::StartBattle()
{
	if (!GetPlayerArmy() || !GetPlayerArmy()->HasUnits())
	{
		AddLogBoardMes("You have no units. Recruit units first.");
		return;
	}
	OnBattleStarted();
	PrepareEnemyArmy();
	UE_LOG(LogRPG, Display, TEXT("[GameMode] Starting war"));
	if (IsBattlePssible())
	{		
		GetWorldTimerManager().SetTimer(BattleTimerHandle, this, &AMyRPGGameMode::ExecuteBattleTurn, 1.0f, true);
	}
}

void AMyRPGGameMode::NextStage()
{
	UE_LOG(LogRPG, Display, TEXT("[GameMode] Next Stage"));
	IsBattleWon = false;
	if (PlayerIsSanta) {
		ElfArmy->ClearArmy();
	}
	else {
		SantaArmy->ClearArmy();
	}
	if (!(GetPlayerArmy()->HasUnits()) && Gold < UnitCost) {
		if (CurrentStage < 10) {
			EndGame(false);
		}
		else {
			EndGame(true);
		}
	}
}

void AMyRPGGameMode::ResolvePlayerAttack(bool bQTESuccess)
{
	bWaitingForQTE = false;	

	if (!ActivePlayerUnit || !ActiveEnemyUnit) {
		return;
	}

	ExecuteDuel(ActivePlayerUnit, ActiveEnemyUnit,bQTESuccess);
}

UArmyComponent* AMyRPGGameMode::GetPlayerArmy() const
{
	return PlayerIsSanta ? SantaArmy : ElfArmy;
}

void AMyRPGGameMode::OnPlayerDealDamage(ABaseRPGCharacter* Attacker, ABaseRPGCharacter* Target)
{
	if (!IsValid(Target) || !Target->IsAlive())	return;

	Target->ReceiveDamage(Attacker->GetDamage());	
}



void AMyRPGGameMode::OnPlayerAttackFinished(ABaseRPGCharacter* Attacker, ABaseRPGCharacter* Target)
{
	
	Attacker->OnAttackFinishedEvent.RemoveDynamic(this, &AMyRPGGameMode::OnPlayerAttackFinished);
	Attacker->OnDamageEvent.RemoveDynamic(this, &AMyRPGGameMode::OnPlayerDealDamage);
	
	if (!Target->IsAlive()) {
		HandleUnitDeath(Target);
		EndTurn();
		return;
	}

	Target->OnAttackFinishedEvent.AddUniqueDynamic(this, &AMyRPGGameMode::OnEnemyAttackFinished);
	Target->OnDamageEvent.AddUniqueDynamic(this, &AMyRPGGameMode::OnPlayerDealDamage);
	Target->AttackTarget(Attacker,Target->RollDamage(8));	
}

void AMyRPGGameMode::OnEnemyAttackFinished(ABaseRPGCharacter* Attacker,ABaseRPGCharacter* Target)
{
	Attacker->OnAttackFinishedEvent.RemoveDynamic(this, &AMyRPGGameMode::OnEnemyAttackFinished);
	Attacker->OnDamageEvent.RemoveDynamic(this, &AMyRPGGameMode::OnPlayerDealDamage);

	if (!Target->IsAlive())
	{
		HandleUnitDeath(Target);
	}
	EndTurn();
}

void AMyRPGGameMode::EndBattle()
{
	GetWorldTimerManager().ClearTimer(BattleTimerHandle);
	FString Winner = SantaArmy->HasUnits() ? TEXT("SANTAS") : TEXT("ELFS");	
	AddLogBoardMes("The winer is " + Winner);

	if (PlayerIsSanta) {
		ElfArmy->ClearArmy();
	}
	else {
		SantaArmy->ClearArmy();
	}

	bool Win = (Winner == "SANTAS") ? true : false;
	if ((Win == true && PlayerIsSanta) || (Win == false && !PlayerIsSanta)) {
		CurrentStage++;
		IsBattleWon = true;
		Gold += 50;
	}
	else {
		Gold += 10;
	}
	NextStage();
	BattleArena->DeactivateBattleCamera(GetWorld()->GetFirstPlayerController());
	if (QuestManager) {
		QuestManager->ProgressActiveQuests();
	}	

	ActiveEnemyUnit = nullptr;
	ActivePlayerUnit = nullptr;
}

void AMyRPGGameMode::HandleUnitDeath(ABaseRPGCharacter* DeadUnit)
{
	if (!IsValid(DeadUnit))
		return;

	AddLogBoardMes(DeadUnit->GetName() + FString(" died"));	
	DeadUnit->HandleDeath(DeadUnit);

	SantaArmy->CleanupDeadUnits();
	ElfArmy->CleanupDeadUnits();

	if (!SantaArmy->HasUnits() || !ElfArmy->HasUnits())
	{
		EndBattle();
	}
}

ABaseRPGCharacter* AMyRPGGameMode::GetCurrentPlayer()
{
	ABaseRPGCharacter* Player = PlayerIsSanta ? SantaArmy->GetActiveUnit() : ElfArmy->GetActiveUnit();
	return Player;
}

ABaseRPGCharacter* AMyRPGGameMode::GetCurrentEnemy()
{
	ABaseRPGCharacter* Enemy= PlayerIsSanta ? ElfArmy->GetActiveUnit() : SantaArmy->GetActiveUnit();
	return Enemy;
}

AMyPlayerController* AMyRPGGameMode::GetMyPC()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC);
	return MyPC;
}

void AMyRPGGameMode::EndTurn()
{
	
	if (ActivePlayerUnit && ActivePlayerUnit->IsAlive()) {
		ActivePlayerUnit->SetActorLocation(LastPlayerCombatLoc);
		ActivePlayerUnit->SetActorEnableCollision(true);		
	}

	if (ActiveEnemyUnit && ActiveEnemyUnit->IsAlive()) {
		ActiveEnemyUnit->SetActorLocation(LastEnemyCombatLoc);
		ActiveEnemyUnit->SetActorEnableCollision(true);		
	}

	battleInProgress = false;
	bWaitingForQTE = false;
	GetWorldTimerManager().UnPauseTimer(BattleTimerHandle);	
}

bool AMyRPGGameMode::IsBattlePssible()
{
	return (SantaArmy && SantaArmy->HasUnits() && ElfArmy && ElfArmy->HasUnits());
}

void AMyRPGGameMode::AddLogBoardMes(const FString& Message)
{
	AMyPlayerController* PC = GetMyPC();
	if (PC) {
		PC->AddLog(Message);
	}
}

void AMyRPGGameMode::EndGame(bool isWon)
{
	AMyPlayerController* PC = GetMyPC();
	PC->ShowEndScreen();
	PC->HideHUD();
	GameResult = isWon;
}

void AMyRPGGameMode::RestartGame()
{
	ActivePlayerUnit = nullptr;
	ActiveEnemyUnit = nullptr;
	SantaArmy->ClearArmy();
	ElfArmy->ClearArmy();
	CurrentStage = 1;
	Gold = 100;
	IsBattleWon = false;
	battleInProgress = false;
}





