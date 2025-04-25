// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "MyAIController.h"
#include "blackboard_keys.h"
#include "MyTestCharacter_2.h"
#include "PlayerCharacter.h"

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


AMyTestGameAIController::AMyTestGameAIController(FObjectInitializer const& object_intializer)
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("Script/AIModule.BehaviorTree'/Game/_My/AI/BT_AIBOT.BT_AIBOT'"));
	if (obj.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Loaded Successfully12313123123"));
		Btree = obj.Object;
	}
	BehaviorTreeComp = object_intializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_intializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	
	SetupPerceptionSystem();

}

void AMyTestGameAIController::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(Btree);
	BehaviorTreeComp->StartTree(*Btree);
	
}

void AMyTestGameAIController::OnPossess(APawn* myPawn)
{
	Super::OnPossess(myPawn);
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
	}
}

void AMyTestGameAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMyTestGameAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//UE_LOG(LogTemp, Warning, TEXT("before ontarget Loaded Successfully"));
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyTestGameAIController::OnTargetDetected);
	//문제있는코드-> 실행이안댐
	//UE_LOG(LogTemp, Warning, TEXT("after ontarget Loaded Successfully"));
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

FRotator AMyTestGameAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw,0.0f);
}

void AMyTestGameAIController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("ontarget Loaded Successfully"));
	
	if (auto const ch = Cast<APlayerCharacter>(actor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Find Player!!!!!!!!!!!!!!");
		GetBlackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
	
}

UBlackboardComponent* AMyTestGameAIController::GetBlackboard() const
{
	return Blackboard;
}
/*
void AMyTestGameAIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Run!");
	}
	bIsPlayerDetected = true;
//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Run!");
}
*/

void AMyTestGameAIController::SetupPerceptionSystem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "33333333333333");
	UE_LOG(LogTemp, Warning, TEXT("setup Loaded Successfully"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	

}
