// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "AIController_Magician.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;


UCLASS()
class MYTESTGAME_2_API AAIController_Magician : public AAIController
{
	GENERATED_BODY()
	
public:

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================


	//=========================================================================
	// FUCTIONS
	//=========================================================================
	AAIController_Magician(FObjectInitializer const& object_intializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* Pawn)override;
	virtual void Tick(float DeltaSeconds)override;
	virtual void PostInitializeComponents() override;

	virtual FRotator GetControlRotation()const override;

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	class UBlackboardComponent* GetBlackboard3() const;

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

	void SetupPerceptionSystem3();
};
