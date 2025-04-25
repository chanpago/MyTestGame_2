// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill3.generated.h"

class BossSill2_Actor;

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_BossSkill3 : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_BossSkill3(const FObjectInitializer& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ABossSill2_Actor> SkillActorClass;
	

private:
	void SpawnSkillActor(UBehaviorTreeComponent* owner_comp);
};
