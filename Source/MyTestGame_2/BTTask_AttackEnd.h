// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackEnd.generated.h"

UCLASS()
class MYTESTGAME_2_API UBTTask_AttackEnd : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	UBTTask_AttackEnd(const FObjectInitializer& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;
};
