// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHitDamage.h"
#include "Components/TextBlock.h"

UPlayerHitDamage::UPlayerHitDamage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerHitDamage::SetHitDamage(float const value)
{
    if (DamageText)
    {
        DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), value)));
        UE_LOG(LogTemp, Warning, TEXT("DamageText is not null!"));

        
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DamageText is null!"));
    }
}

void UPlayerHitDamage::PlayDamageTextAnimation()
{
    if (DamageTextAnimation)
    {
        PlayAnimation(DamageTextAnimation);
    }
}

