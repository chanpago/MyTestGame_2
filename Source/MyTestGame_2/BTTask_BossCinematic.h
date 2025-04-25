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

    // 타이머 핸들
    FTimerHandle TimerHandle;

    // 타이머가 완료되었을 때 호출될 함수
    void OnTimerFinished(UBehaviorTreeComponent* OwnerComp);
};
