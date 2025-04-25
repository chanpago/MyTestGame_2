// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidget.h"

#include "MyBotCharacter_3.h"

void UBossWidget::NativeConstruct()
{
}

void UBossWidget::SetBossHPBarValuePercent(float const value)
{
	BossHPBar->SetPercent(value);
}

void UBossWidget::SetBossName(FText BossName)
{
	BossNameText->SetText(BossName);
}

