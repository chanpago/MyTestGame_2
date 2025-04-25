// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPWidget.h"

void UBossHPWidget::SetHPBarValuePercent(float const value)
{
	BossHPBar->SetPercent(value);
}
