// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyTestGame_2GameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GameStart,
	GamePlay,
	GamePause,
	GameOver,
};

UCLASS()
class MYTESTGAME_2_API AMyTestGame_2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, Category = "UMG Game")
	EGameState GameStateEnum;

	UPROPERTY(EditAnyWhere, BlueprintREadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget>HUDWidgetClass;

	UUserWidget* CurrentWidget;

public:
	void ChangeUI();

};
