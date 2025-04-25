// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBTService_RandomNumber : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
