// Fill out your copyright notice in the Description page of Project Settings.


//game
#include "MyAIBossController.h"
#include "MyTestCharacter_2.h"
#include "PlayerCharacter.h"
#include "blackboard_keys.h"
//engine

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Engine.h"


AMyAIBossController::AMyAIBossController(FObjectInitializer const& object_intializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("Script/AIModule.BehaviorTree'/Game/_My/AI/BT_AIBoss.BT_AIBoss'"));
	if (obj.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Loaded Successfully"));
		Btree = obj.Object;
	}
	BehaviorTreeComp = object_intializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_intializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config2"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component2")));
	SetupPerceptionSystem2();
}

void AMyAIBossController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(Btree);
	BehaviorTreeComp->StartTree(*Btree);

}

void AMyAIBossController::OnPossess(APawn* myPawn)
{
	Super::OnPossess(myPawn);
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
	}
}

void AMyAIBossController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMyAIBossController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIBossController::OnTargetDetected);
	
}

FRotator AMyAIBossController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AMyAIBossController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{

	if (auto const ch = Cast<APlayerCharacter>(actor))
	{
		GetBlackboard2()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}

}

UBlackboardComponent* AMyAIBossController::GetBlackboard2() const
{
	return Blackboard;
}


void AMyAIBossController::SetupPerceptionSystem2()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "setup Loaded Successfully!!!!");
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "44444444444444444");
	UE_LOG(LogTemp, Warning, TEXT("setup Loaded Successfully"));
	

	if (SightConfig) {
		SightConfig->SightRadius = 2000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
		SightConfig->PeripheralVisionAngleDegrees = 120.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 1520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("setup not Loaded Successfully"));
	}

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//UE_LOG(LogTemp, Warning, TEXT("before ontarget Loaded Successfully"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "before ontarget Loaded Successfully!!!!");
	

	//문제있는코드-> 실행이안됨
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "after ontarget Loaded Successfully!!!!");
	//UE_LOG(LogTemp, Warning, TEXT("after ontarget Loaded Successfully"));
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
