// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAttackAnimInstance.h"
#include "GameFramework/Character.h"  
#include "PlayerCharacter.h"          
#include "Engine/Engine.h"


void UComboAttackAnimInstance::AnimNotify_DamageStart()
{
    // �÷��̾� ĳ���� ��������
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {
        // �÷��̾� ĳ���Ͱ� ���������� ���������� �� ������ �ڵ�
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_DamageStart!");
        PlayerCharacter->DamageApply = true;  // ĳ���Ϳ��� ȣ���ϰ� ���� �Լ� ����
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}

void UComboAttackAnimInstance::AnimNotify_DamageEnd()
{
    // �÷��̾� ĳ���� ��������
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter)
    {
        // �÷��̾� ĳ���Ͱ� ���������� ���������� �� ������ �ڵ�
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_DamageEnd!");
        PlayerCharacter->DamageApply = false;  // ĳ���Ϳ��� ȣ���ϰ� ���� �Լ� ����
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
        // �÷��̾� ĳ���Ͱ� ���������� ���������� �� ������ �ڵ�
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_RollEnd!");
        PlayerCharacter->SetCannotDamaged(false);
        PlayerCharacter->IsRolling = false;
       
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get PlayerCharacter!");
    }
}
