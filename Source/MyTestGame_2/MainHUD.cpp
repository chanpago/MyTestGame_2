// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "MainHUD.h"
#include "MainMenu.h"
#include "HpExpBar.h"
#include "GameStartMenu.h"
#include "PlayerCharacter.h"
#include "FirstWeaponSlot.h"
#include "ItemBase.h"
#include "BossWidget.h"
#include "MyBotCharacter_3.h"
#include "BossDieWidget.h"

//engine
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionWidget.h"
#include "GameInstance_Player.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AMainHUD::AMainHUD()
{
}

void AMainHUD::InitializeUI()
{
	
	
}


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();



	APlayerController* PlayerController = GetOwningPlayerController();
	PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());

	// BossCharacterClass로 보스 캐릭터를 검색하고 BossCharacter에 할당
	if (BossCharacterClass)
	{
		BossCharacter = Cast<AMyBotCharacter_3>(UGameplayStatics::GetActorOfClass(GetWorld(), BossCharacterClass));
		if (!BossCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("BossCharacter not found with specified class"));
		}
	}



	UGameInstance_Player* GameInstancePlayer = Cast<UGameInstance_Player>(GetGameInstance());
	/*
	if (GameStartMenuWidgetClass)
	{
		GameStartMenuWidget = CreateWidget<UGameStartMenu>(GetWorld(), GameStartMenuWidgetClass);
		GameStartMenuWidget->AddToViewport(6);
		GameStartMenuWidget->SetVisibility(ESlateVisibility::Visible);

	}
	*/

	if (HpExpWidgetClass)
	{
		if (GameInstancePlayer->bIsGameStartMenuDisplayed)
		{
			HpExpWidget = CreateWidget<UHpExpBar>(GetWorld(), HpExpWidgetClass);
			HpExpWidget->AddToViewport(0);
			HpExpWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			HpExpWidget = CreateWidget<UHpExpBar>(GetWorld(), HpExpWidgetClass);
			HpExpWidget->AddToViewport(0);
			HpExpWidget->SetVisibility(ESlateVisibility::Collapsed);
			UE_LOG(LogTemp, Warning, TEXT("HpExpWidgetClass"));
		}

	}


	if (GameInstancePlayer && !GameInstancePlayer->bIsGameStartMenuDisplayed && GameStartMenuWidgetClass)
	{
		GameStartMenuWidget = CreateWidget<UGameStartMenu>(GetWorld(), GameStartMenuWidgetClass);
		if (GameStartMenuWidget)
		{
			GameStartMenuWidget->AddToViewport(6);
			GameStartMenuWidget->SetVisibility(ESlateVisibility::Visible);
			GameInstancePlayer->bIsGameStartMenuDisplayed = true; // 최초 한 번만 생성 및 표시
		}
	}
	if (MainMenuClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("MainMenuClass ok"));
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetClass"));
	}
	
	
	if (BossWidgetClass)
	{
		BossWidget = CreateWidget<UBossWidget>(GetWorld(), BossWidgetClass);
		BossWidget->AddToViewport(4);
		BossWidget->SetVisibility(ESlateVisibility::Collapsed);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BossWidget ok"));
	}
	
	if (BossDieWidgetClass)
	{
		BossDieWidget = CreateWidget<UBossDieWidget>(GetWorld(), BossDieWidgetClass);
		BossDieWidget->AddToViewport(7);
		BossDieWidget->SetVisibility(ESlateVisibility::Collapsed);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BossWidget ok"));
	}

	
	
}

void AMainHUD::Tick(float DeltaSeconds)
{

	if (PlayerCharacter)
	{
		//PlayerCharacter->MyHealth;
		PlayerCharacter->MyHPnum = (PlayerCharacter->MyHealth / PlayerCharacter->MyMaxHealth * 100) * 0.01f;
		PlayerCharacter->MyEXPnum = (PlayerCharacter->MyEXP / PlayerCharacter->MyMaxEXP * 100) * 0.01f;
		PlayerCharacter->Staminanum = (PlayerCharacter->NowStamina / PlayerCharacter->MaxStamina * 100) * 0.01f;

		HpExpWidget->SetStaminaBarValuePercent(PlayerCharacter->Staminanum);
		HpExpWidget->SetHPBarValuePercent(PlayerCharacter->MyHPnum);
		ItemReference = PlayerCharacter->GetWeaponReference();
		if (ItemReference) {
			HpExpWidget->Weapon1->SetBrushFromTexture(ItemReference->AssetData.Icon);
			if (!ItemReference->bIsUsed) {
				UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
				HpExpWidget->Weapon1->SetBrushFromTexture(Texture);
			}
		}
		
		
		
		//HpExpWidget->SetWeapon1Icon();
	}
	
	if (BossCharacter)
	{

		BossCharacter->MyHPnum = (BossCharacter->MyHealth / BossCharacter->MyMaxHealth * 100) * 0.01f;
		BossWidget->SetBossHPBarValuePercent(BossCharacter->MyHPnum);
		BossWidget->SetBossName(BossCharacter->BossName);
	}
	
	if (BossCharacter->MyHealth <= 0) {
		BossWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}


void AMainHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AMainHUD::DisPlayBossUI()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DisPlayBossUI"));
	if (bBossWidgetVisible)
	{
		if (BossWidget)
		{
			bBossWidgetVisible = false;
			BossWidget->SetVisibility(ESlateVisibility::Collapsed);
		}		
	}
	else
	{
		///GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("else On"));
		if (BossWidget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Boss Widget On"));
			//bBossWidgetVisible = true;
			BossWidget->SetVisibility(ESlateVisibility::Visible);
		}		
	}
}

void AMainHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		//UE_LOG(LogTemp, Log, TEXT("Hide!"));
		
	}
}

void AMainHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AMainHUD::BossDieDisplay()
{
	BossDieWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMainHUD::HideBossDieDisplay()
{
	BossDieWidget->SetVisibility(ESlateVisibility::Collapsed);
}

