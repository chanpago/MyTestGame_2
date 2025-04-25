// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyTestCharacter_2.h"
#include "Components/WrapBox.h"
#include "InventoryComponent.h"
#include "GameInstance_Player.generated.h"

/**
 * 
 */
class UInventoryItemSlot;
class UItemBase;
class APlayerCharacter;
class AMyTestWeapon;

UCLASS()
class MYTESTGAME_2_API UGameInstance_Player : public UGameInstance
{
	GENERATED_BODY()
	

public:
    //=========================================================================
    // FUCTIONS
    //=========================================================================
    UGameInstance_Player();

    // �÷��̾� ������ ���� �� �ҷ����� �Լ�
    void SavePlayerData();
    void LoadPlayerData();
    
    void SetPlayerReference(APlayerCharacter* PlayerCharacter);

    //=========================================================================
    // PROPERTIES & VARIABLES
    //=========================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
    int32 PlayerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
    float PlayerExperience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
    float PlayerHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
    UInventoryComponent* PlayerInventory;

    UPROPERTY()
    APlayerCharacter* Player;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryPanel;

    UPROPERTY()
    TArray<UItemBase*> SavedInventoryItems;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UInventoryItemSlot> InventorySlotClass;

    UPROPERTY()
    TSubclassOf<AMyTestWeapon> MyFirstWeapon;

    UPROPERTY()
    TSubclassOf<AMyTestWeapon> MySecondWeapon;

    // ���� ���� �޴��� �̹� ǥ�õǾ����� ���θ� �����ϴ� �÷���
    UPROPERTY(BlueprintReadWrite, Category = "Game State")
    bool bIsGameStartMenuDisplayed;
   
};
