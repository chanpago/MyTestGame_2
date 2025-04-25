// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "blackboard_keys.h"
#include "PlayerCharacter.h"
#include "MyBotCharacter_3.h"

//engine
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "BTTask_BossCinematic.h"

UBTTask_BossCinematic::UBTTask_BossCinematic()
{
	NodeName = TEXT("Play Boss Cinematic");
}

EBTNodeResult::Type UBTTask_BossCinematic::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    auto const AIController = OwnerComp.GetAIOwner();
    AMyBotCharacter_3* const Boss = Cast<AMyBotCharacter_3>(AIController->GetPawn());
  
    /*
     1. ���� ������ ���
    if (LevelSequenceActor.IsValid() && LevelSequenceActor->SequencePlayer)
    {
        LevelSequenceActor->SequencePlayer->Play();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelSequenceActor or SequencePlayer is not valid"));
    }
    */
    // 2. �ִϸ��̼� ���

    
    if (AnimMontage)
    {
        float AnimationDuration = AnimMontage->GetPlayLength();
        Boss->PlayAnimMontage(AnimMontage);

        // Ÿ�̸� ����
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_BossCinematic::OnTimerFinished, &OwnerComp);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AnimationDuration, false);

        return EBTNodeResult::InProgress;
    } 
    return EBTNodeResult::Failed;
}


void UBTTask_BossCinematic::OnTimerFinished(UBehaviorTreeComponent* OwnerComp)
{
    // ������ Ű ����
    UBlackboardComponent* BlackboardComp = OwnerComp->GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(FName(TEXT("BossStart")), true);
    }

    if (OwnerComp) {
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
