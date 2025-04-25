// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

class AMyBotCharacter_2;
class AMyBotCharacter_3;

UCLASS()
class MYTESTGAME_2_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	UBTTask_Attack(const FObjectInitializer& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	bool MontageHasFinished(const AMyBotCharacter_2* NpcBot);

	bool MontageHasFinished(const AMyBotCharacter_3* NpcBot);

};
