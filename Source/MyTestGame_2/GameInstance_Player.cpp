// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "GameInstance_Player.h"
#include "MyTestCharacter_2.h"
#include "InventoryItemSlot.h"
#include "ItemBase.h"
#include "PlayerCharacter.h"

//engine


UGameInstance_Player::UGameInstance_Player() : bIsGameStartMenuDisplayed(false)
{
    // 생성자에서 데이터 초기화 등을 수행할 수 있습니다.
    PlayerLevel = 1;
    PlayerExperience = 0.0f;
    PlayerHealth = 50.0f;
}

void UGameInstance_Player::SavePlayerData()
{
    PlayerLevel = Player->MyLevel;
    PlayerExperience = Player->MyEXP;
    PlayerHealth = Player->MyHealth;

    // 인벤토리 아이템 저장
    SavedInventoryItems.Empty();
    if (Player->PlayerInventory)
    {
        for (auto& Item : Player->PlayerInventory->GetInventoryContents())
        {
            SavedInventoryItems.Add(Item);
        }
    }
}

void UGameInstance_Player::LoadPlayerData()
{
    Player->MyLevel = PlayerLevel;
    Player->MyEXP = PlayerExperience;
    Player->MyHealth = PlayerHealth;
    
    if (Player->PlayerInventory)
    {
        //Player->PlayerInventory->ClearInventory();
        for (auto& Item : SavedInventoryItems)
        {
            Player->PlayerInventory->AddNewItem(Item, Item->Quantity);
        }
    }
    
}

void UGameInstance_Player::SetPlayerReference(APlayerCharacter* PlayerCharacter)
{
    Player = PlayerCharacter;
}

