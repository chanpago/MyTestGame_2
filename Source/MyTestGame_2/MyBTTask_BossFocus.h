// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_BossFocus.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UMyBTTask_BossFocus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	UMyBTTask_BossFocus(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_Comp, uint8* NodeMemory);
};
