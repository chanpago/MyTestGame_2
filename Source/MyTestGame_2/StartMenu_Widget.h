// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "StartMenu_Widget.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UStartMenu_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BtnStartGame = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BtnExitGame = nullptr;

private:
	UFUNCTION()
	void ButtonStartGameClicked();

	UFUNCTION()
	void ButtonExitGameClicked();
};
