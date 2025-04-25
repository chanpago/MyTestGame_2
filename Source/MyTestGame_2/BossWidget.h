// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Components/TextBlock.h" 
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void NativeConstruct() override;

	// 보스 체력바의 값을 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SetBossHPBarValuePercent(float const value);

	// 보스 이름을 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SetBossName(FText BossName);


protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* BossHPBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* BossNameText = nullptr;
	
};
