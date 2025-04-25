// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_Player.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API USaveGame_Player : public USaveGame
{
	GENERATED_BODY()
	

public:
	void SavePlayerData();
	void LoadPlayerData();

};
