// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyAIBossController.generated.h"

/**
 *
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class  AMyAIBossController : public AAIController
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	AMyAIBossController(FObjectInitializer const& object_intializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* Pawn)override;
	virtual void Tick(float DeltaSeconds)override;
	virtual void PostInitializeComponents() override;

	virtual FRotator GetControlRotation()const override;

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	class UBlackboardComponent* GetBlackboard2() const;

private:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Btree;

	class UBlackboardComponent* Blackboard;
	class UAISenseConfig_Sight* SightConfig;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void SetupPerceptionSystem2();
};