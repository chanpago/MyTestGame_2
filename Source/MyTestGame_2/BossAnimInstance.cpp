// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "GameFramework/Character.h"
#include "MyBotCharacter_3.h"

void UBossAnimInstance::AnimNotify_AttackStart()
{
    // 소유 캐릭터 가져오기
    ACharacter* OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (OwnerCharacter)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Success to get OwnerCharacter!");
        AMyBotCharacter_3* BossCharacter = Cast<AMyBotCharacter_3>(OwnerCharacter);
        if (BossCharacter)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_DamageStart!");
            BossCharacter->SetCanDamaged(true);
        }
    }
    else {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get OwnerCharacter!");
    }
}

void UBossAnimInstance::AnimNotify_AttackEnd()
{
    // 소유 캐릭터 가져오기
    ACharacter* OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (OwnerCharacter)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Success to get OwnerCharacter!");
        AMyBotCharacter_3* BossCharacter = Cast<AMyBotCharacter_3>(OwnerCharacter);
        if (BossCharacter)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AnimNotify_AttackEnd!");
            BossCharacter->SetCanDamaged(false);
        }
    }
    else {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to get OwnerCharacter!");
    }
}
