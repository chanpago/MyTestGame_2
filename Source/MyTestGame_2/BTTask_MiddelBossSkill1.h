// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MiddelBossSkill1.generated.h"


class AMyBotCharacter_3;

UCLASS()
class MYTESTGAME_2_API UBTTask_MiddelBossSkill1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_MiddelBossSkill1(FObjectInitializer const& object_initializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector SkillActorKey;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class AMiddleSkill1IceActor> SkillActorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillSpeed = 3000.f;

	AMyBotCharacter_3* MiddleBoss;


private:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void SpawnSkillActor(UBehaviorTreeComponent* owner_comp);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	FTimerHandle TimerHandle;
};
