//game
#include "AIController_Magician.h"
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

AAIController_Magician::AAIController_Magician(FObjectInitializer const& object_intializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Behavior Tree 로드
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("Script/AIModule.BehaviorTree'/Game/_My/AI/BT_AIBOT2.BT_AIBOT2'"));
	if (obj.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Loaded Successfully2342342424"));
		Btree = obj.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Behavior Tree23423423423"));
	}

	BehaviorTreeComp = object_intializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_intializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));


	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config3"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component3")));
	SetupPerceptionSystem3();
}

void AAIController_Magician::BeginPlay()
{
	Super::BeginPlay();

	if (Btree)
	{
		RunBehaviorTree(Btree);
		BehaviorTreeComp->StartTree(*Btree);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Behavior Tree is null in BeginPlay"));
	}
}

void AAIController_Magician::OnPossess(APawn* myPawn)
{
	Super::OnPossess(myPawn);

	if (Blackboard && Btree)
	{
		Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
	}
	else
	{
		if (!Blackboard)
		{
			UE_LOG(LogTemp, Error, TEXT("Blackboard is null in OnPossess"));
		}
		if (!Btree)
		{
			UE_LOG(LogTemp, Error, TEXT("Behavior Tree is null in OnPossess"));
		}
	}
}

void AAIController_Magician::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAIController_Magician::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	UE_LOG(LogTemp, Error, TEXT("PostInitializeComponents"));
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Magician::OnTargetDetected);
	//GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

FRotator AAIController_Magician::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAIController_Magician::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	
	if (auto const ch = Cast<APlayerCharacter>(actor))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Find Player12313123131");
		GetBlackboard3()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AAIController_Magician OnTargetDetected");
		UE_LOG(LogTemp, Error, TEXT("AAIController_Magician OnTargetDetected"));
	}
}

UBlackboardComponent* AAIController_Magician::GetBlackboard3() const
{
	return Blackboard;
}

void AAIController_Magician::SetupPerceptionSystem3()
{
	UE_LOG(LogTemp, Warning, TEXT("setup Loaded Successfully"));
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 300.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("setup not Loaded Successfully"));
	}

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//UE_LOG(LogTemp, Warning, TEXT("before ontarget Loaded Successfully"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "before ontarget Loaded Successfully!!!!");


	//문제있는코드-> 실행이안댐
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "after ontarget Loaded Successfully!!!!");
	//UE_LOG(LogTemp, Warning, TEXT("after ontarget Loaded Successfully"));
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}