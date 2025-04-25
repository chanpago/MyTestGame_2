// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
class APlayerCharacter;
class UGameStartMenu;
class UHpExpBar;
class UFirstWeaponSlot;
class UBossWidget;
class AMyBotCharacter_3;
class UBossDieWidget;

UCLASS()
class MYTESTGAME_2_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHpExpBar> HpExpWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGameStartMenu> GameStartMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UFirstWeaponSlot> FirstWeaponSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UBossWidget> BossWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UBossDieWidget> BossDieWidgetClass;

	UPROPERTY()
	UItemBase* ItemReference;

	bool bIsMenuVisible;

	bool bBossWidgetVisible = false;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "boss")
	TSubclassOf<AMyBotCharacter_3> BossCharacterClass;
	
	AMyBotCharacter_3* BossCharacter;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UHpExpBar* HpExpWidget;
	
	UPROPERTY()
	UBossWidget* BossWidget;

	UPROPERTY()
	UBossDieWidget* BossDieWidget;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	AMainHUD();

	void InitializeUI();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();
	void DisPlayBossUI();

	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;


	void BossDieDisplay();

	void HideBossDieDisplay();
	
protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UGameStartMenu* GameStartMenuWidget;


	//=========================================================================
	// FUCTIONS
	//=========================================================================

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
