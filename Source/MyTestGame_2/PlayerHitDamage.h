// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHitDamage.generated.h"

class UTextBlock;
class AMyBasicCharacter_2;
/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UPlayerHitDamage : public UUserWidget
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UPlayerHitDamage(const FObjectInitializer& ObjectInitializer);

	void SetHitDamage(float const value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PlayDamageTextAnimation();

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	AMyBasicCharacter_2* Mypawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DamageText = nullptr;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageTextAnimation;

};
