// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Components/TextBlock.h" 
#include "HpExpBar.generated.h"

/**
 * 
 */
class UItemBase;
class UImage;
class APlayerCharacter;
class UTextBlock;

UCLASS()
class MYTESTGAME_2_API UHpExpBar : public UUserWidget
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void NativeConstruct() override;

	void SetHPBarValuePercent(float const value);

	void SetStaminaBarValuePercent(float const value);

	void SetWeapon1Icon(UItemBase* Item);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Weapon1;

protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* StaminaBar = nullptr;

	UPROPERTY()
	UItemBase* ItemReference;
};
