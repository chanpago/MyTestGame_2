// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LevelSequenceActor.h"
#include "BTTask_BossCinematic.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_BossCinematic : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
    UBTTask_BossCinematic();



    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* AnimMontage;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

    // Ÿ�̸� �ڵ�
    FTimerHandle TimerHandle;

    // Ÿ�̸Ӱ� �Ϸ�Ǿ��� �� ȣ��� �Լ�
    void OnTimerFinished(UBehaviorTreeComponent* OwnerComp);
};
