// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "HUD_InGame.generated.h"


/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API AHUD_InGame : public AHUD
{
	GENERATED_BODY()
	
public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	AHUD_InGame();
	void BeginPlay() override;
	void SetHpBarValuePercent(float const value);
	void SetExpBarValuePercent(float const value);

private:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	TSubclassOf<class UUserWidget> UiBPClass;
	class UUserWidget* UiWidget;

protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ExpBar = nullptr;
};
