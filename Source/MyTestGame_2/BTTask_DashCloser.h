// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DashCloser.generated.h"


class UBlackboardComponent;
class AMyBotCharacter_3;
/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_DashCloser : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_DashCloser(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	class UBlackboardComponent* Blackboard;

private:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void StartDashAnimation(AMyBotCharacter_3* AICharacter);
	void StopDashAnimation(AMyBotCharacter_3* AICharacter);
};
