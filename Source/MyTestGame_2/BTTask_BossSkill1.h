// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossSkill1.generated.h"

class AMyIceHone;
class ABossSill1_Actor;
class AMyBossCharacter;

/**
 *
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_BossSkill1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_BossSkill1(const FObjectInitializer& object_intializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;
	void DelayedTask();

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	TSubclassOf<class AMyIceHone> MyIceAttack;
	AMyIceHone* MyIceInstance;
	FTimerHandle TimerHandle_Delay;

private:
	bool MontageHasFinished(const AMyBossCharacter* Boss);

};
