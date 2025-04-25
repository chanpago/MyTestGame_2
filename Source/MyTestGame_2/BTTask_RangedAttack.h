// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_RangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_RangedAttack(const FObjectInitializer& object_intialize);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

	UPROPERTY(EditAnywhere, Category = Pawn)
	UAnimMontage* RangedAttackAnim1;

	UPROPERTY(EditAnywhere, Category = Pawn)
	UAnimMontage* RangedAttackAnim2;


private:

	// 타이머 핸들
	FTimerHandle TimerHandle;

	// 타이머가 완료되었을 때 호출될 함수
	void OnTimerFinished(UBehaviorTreeComponent* owner_comp);
};
