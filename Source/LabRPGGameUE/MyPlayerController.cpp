#include "MyPlayerController.h"
#include "MyRPGGameMode.h"

AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerController::ToggleArmyMenu()
{
	if (ArmyMenuWidget && ArmyMenuWidget->IsInViewport()) {
		ArmyMenuWidget->RemoveFromParent();
		ArmyMenuWidget = nullptr;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);		
	}
	else {
		if (ArmyMenuClass) {
			ArmyMenuWidget = CreateWidget<UUserWidget>(this, ArmyMenuClass);
			if (ArmyMenuWidget) {
				ArmyMenuWidget->AddToViewport();

				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(ArmyMenuWidget->TakeWidget());
				SetInputMode(InputMode);
				bShowMouseCursor = true;
			}
		}
	}
}

void AMyPlayerController::ShowMenu()
{
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuClass);
		if (MainMenuWidget)
		{			
			MainMenuWidget->AddToViewport();			
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			SetInputMode(InputMode);
			
		}
	}
	
}

void AMyPlayerController::HideMenu()
{
	if (MainMenuWidget) {		
		MainMenuWidget->RemoveFromParent();
	}	
}

void AMyPlayerController::ShowHUD()
{

	if (GameHUDClass) {
		GameHUDWidget = CreateWidget<UUserWidget>(this, GameHUDClass);
		if (GameHUDWidget) {
			GameHUDWidget->AddToViewport();			
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(GameHUDWidget->TakeWidget());
			SetInputMode(InputMode);			
		}
	}
	if (LogBoardClass)
	{
		LogBoardWidget = CreateWidget<UUserWidget>(this, LogBoardClass);
		if (LogBoardWidget)
		{
			LogBoardWidget->AddToViewport();
		}
	}
}

void AMyPlayerController::HideHUD()
{
	if (GameHUDWidget) {		
		GameHUDWidget->RemoveFromParent();
	}	
}

void AMyPlayerController::StartQTE()
{
	if (QTEAttackClass) {
		QTEAttackWidget = CreateWidget<UUserWidget>(this, QTEAttackClass);
		if (QTEAttackWidget) {
			QTEAttackWidget->AddToViewport();
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = false;
			IsQTEActive = true;
			QTEProgress = 0.0f;
		}
	}
}

void AMyPlayerController::StopQTE()
{
	if (QTEAttackWidget) {
		QTEAttackWidget->RemoveFromParent();		
		IsQTEActive = false;		
	}
	bool bSuccess = (QTEProgress >= 0.7f && QTEProgress <= 0.95f);
	AMyRPGGameMode* GM = Cast<AMyRPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->ResolvePlayerAttack(bSuccess);
	}
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayerController::StopQTE);

}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsQTEActive) {

		QTEProgress += QTESpeed * DeltaTime;

		if (QTEProgress >= 1.2f) {
			StopQTE();
		}
	}

}

void AMyPlayerController::ShowQuestPanel ()
{

	if (QuestClass) {
		QuestWidget = CreateWidget<UUserWidget>(this, QuestClass);
		if (QuestWidget) {
			QuestWidget->AddToViewport();			
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(QuestWidget->TakeWidget());
			SetInputMode(InputMode);

		}
	}
}

void AMyPlayerController::HideQuestPanel()
{
	if (QuestWidget) {
		QuestWidget->RemoveFromParent();
	}	
}

void AMyPlayerController::ShowQuestUnitSelector(UDataAsset* ChosenAsset)
{
	if (QuestUnitSelectorClass) {
		QuestUnitSelectorWidget = CreateWidget<UUserWidget>(this, QuestUnitSelectorClass);
		if (QuestUnitSelectorWidget) {
			QuestUnitSelectorWidget->AddToViewport();			
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(QuestUnitSelectorWidget->TakeWidget());
			SetInputMode(InputMode);

		}
	}

}

void AMyPlayerController::HideQuestUnitSelector()
{

	if (QuestUnitSelectorWidget) {		
		QuestUnitSelectorWidget->RemoveFromParent();
	}	
}

void AMyPlayerController::SetChosenQuest(UQuestAsset* Quest)
{
	ChosenQuest = Quest;
}

UQuestAsset* AMyPlayerController::GetChosenQuest() const
{
	return ChosenQuest;
}

void AMyPlayerController::AddLog(const FString& Message)
{
	if (!LogBoardWidget) return;

	UFunction* Func = LogBoardWidget->FindFunction(FName("AddLog"));
	if (Func)
	{
		struct FParams { FText Text; };
		FParams Params;
		Params.Text = FText::FromString(Message);
		LogBoardWidget->ProcessEvent(Func, &Params);
	}
}

void AMyPlayerController::ShowEndScreen()
{
	if (EndScreenClass) {
		EndScreenWidget = CreateWidget<UUserWidget>(this, EndScreenClass);
		if (EndScreenWidget) {
			EndScreenWidget->AddToViewport();
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(EndScreenWidget->TakeWidget());
			SetInputMode(InputMode);
		}
	}
}

void AMyPlayerController::HideEndScreen()
{
	if (EndScreenWidget) {
		EndScreenWidget->RemoveFromParent();
	}
}

