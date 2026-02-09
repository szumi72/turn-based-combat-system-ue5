#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class LABRPGGAMEUE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMyPlayerController();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleArmyMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHUD();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideHUD();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void StartQTE();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void StopQTE();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	float GetQTEProgress() { return QTEProgress; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowQuestPanel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideQuestPanel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowQuestUnitSelector(UDataAsset* ChosenAsset);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideQuestUnitSelector();
	
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SetChosenQuest(UQuestAsset* Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	UQuestAsset* GetChosenQuest()const;

	void AddLog(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "EndScreen")
	void ShowEndScreen();

	UFUNCTION(BlueprintCallable, Category = "EndScreen")
	void HideEndScreen();

protected:

	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UUserWidget* MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	UUserWidget* GameHUDWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameHUDClass;

	UPROPERTY()
	UUserWidget* ArmyMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ArmyMenuClass;

	UPROPERTY()
	UUserWidget* QTEAttackWidget;

	UPROPERTY(EditAnywhere, Category = "QTE")
	TSubclassOf<UUserWidget>QTEAttackClass;

	UPROPERTY()
	UUserWidget* QuestWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestClass;

	UPROPERTY()
	UUserWidget* QuestUnitSelectorWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestUnitSelectorClass;

	UPROPERTY(EditAnywhere, Category = "Quest")
	UQuestAsset* ChosenQuest;

	UPROPERTY(EditAnywhere, Category = "Log Board")
	TSubclassOf<UUserWidget> LogBoardClass;

	UPROPERTY()
	UUserWidget* LogBoardWidget;

	UPROPERTY(EditAnywhere, Category = "End Screen")
	TSubclassOf<UUserWidget> EndScreenClass;

	UPROPERTY()
	UUserWidget* EndScreenWidget;
	
private:
	bool IsQTEActive = false;
	float QTEProgress = 0.0f;
	float QTESpeed = 0.8f;
	
};
