// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DashAway.generated.h"

class UBlackboardComponent;
class AMyBotCharacter_3;

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_DashAway : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_DashAway(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

	void StartDashAnimation(AMyBotCharacter_3* AICharacter);
	void StopDashAnimation(AMyBotCharacter_3* AICharacter);
	void OnMoveCompleted(AAIController* ai_controller, AMyBotCharacter_3* ai_character, FAIRequestID RequestID);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	class UBlackboardComponent* Blackboard;

};
