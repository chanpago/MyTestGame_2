// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()



public:

	void SetHPBarValuePercent(float const value);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* BossHPBar = nullptr;
	
};
