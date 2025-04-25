// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerAttackRange.generated.h"

UCLASS()
class MYTESTGAME_2_API UBTService_IsPlayerAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	UBTService_IsPlayerAttackRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

};
