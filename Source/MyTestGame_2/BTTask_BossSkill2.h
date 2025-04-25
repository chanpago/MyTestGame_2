// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossSkill2.generated.h"

class AMyBossCharacter;
class AMyIceHone;
/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_BossSkill2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_BossSkill2(const FObjectInitializer& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	TSubclassOf<class AMyIceHone> MyIceAttack;

	AMyIceHone* MyIceInstance;

	
};
