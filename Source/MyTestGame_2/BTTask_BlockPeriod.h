// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BlockPeriod.generated.h"


class AMyBotCharacter_3;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTTask_BlockPeriod : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UBTTask_BlockPeriod(FObjectInitializer const& object_initializer);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	APlayerCharacter* PlayerCharacterInstance;
	UBlackboardComponent* BlackboardCompInstance;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

private:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void MoveToSide(AAIController* ai_controller, ACharacter* ai_character, ACharacter* player_character);
	void StartBlock(AMyBotCharacter_3* ai_character);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void FinishBlock();

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	FTimerHandle BlockTimerHandle; // 타이머 핸들
	float BlockDuration; // 방어 지속 시간
	UBehaviorTreeComponent* OwnerCompInstance; // OwnerComp 인스턴스를 저장할 변수
};
