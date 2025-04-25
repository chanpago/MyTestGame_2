// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "FirstWeaponSlot.h"
#include "ItemBase.h"
#include "PlayerCharacter.h"
#include "MyTestWeapon.h"
#include "HpExpBar.h"

//engine
#include "Components/Image.h"

void UHpExpBar::NativeConstruct()
{
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
	Weapon1->SetBrushFromTexture(Texture);
}

void UHpExpBar::SetHPBarValuePercent(float const value)
{
	HPBar->SetPercent(value);
}

void UHpExpBar::SetStaminaBarValuePercent(float const value)
{
	StaminaBar->SetPercent(value);
}

void UHpExpBar::SetWeapon1Icon(UItemBase* Item)
{


}




