// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAttackAnimInstance.h"
#include "GameFramework/Character.h"  
#include "PlayerCharacter.h"          
#include "Engine/Engine.h"


void UComboAttackAnimInstance::AnimNotify_DamageStart()
{
    // 플레이어 캐릭터 가져오기
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {
        // 플레이어 캐릭터가 정상적으로 가져와졌을 때 실행할 코드
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_DamageStart!");
        PlayerCharacter->DamageApply = true;  // 캐릭터에서 호출하고 싶은 함수 예시
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}

void UComboAttackAnimInstance::AnimNotify_DamageEnd()
{
    // 플레이어 캐릭터 가져오기
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {
        // 플레이어 캐릭터가 정상적으로 가져와졌을 때 실행할 코드
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_DamageEnd!");
        PlayerCharacter->DamageApply = false;  // 캐릭터에서 호출하고 싶은 함수 예시
        PlayerCharacter->AOWNUM = false;
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}

void UComboAttackAnimInstance::AnimNotify_RollStart()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {   
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_RollStart!");
        PlayerCharacter->SetCannotDamaged(true);
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}

void UComboAttackAnimInstance::AnimNotify_RollEnd()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {
        // 플레이어 캐릭터가 정상적으로 가져와졌을 때 실행할 코드
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_RollEnd!");
        PlayerCharacter->SetCannotDamaged(false);
        PlayerCharacter->IsRolling = false;
       
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}
