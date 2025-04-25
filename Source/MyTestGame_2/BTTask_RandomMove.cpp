//game
#include "BTTask_RandomMove.h"
#include "MyTestGame_2.h"
#include "NavigationSystem.h"
#include "MyAIController.h"
#include "MyAIBossController.h"
#include "AIController_Magician.h"
#include "MyBotCharacter_2.h"
#include "MyBotCharacter_3.h"
#include "MyBossCharacter.h"
#include "blackboard_keys.h"

//engine
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine.h"
#include "Engine/World.h"
#include "blackboard_keys.h"


UBTTask_RandomMove::UBTTask_RandomMove(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& owner_Comp, uint8* NodeMemory)
{
    auto const cont = owner_Comp.GetAIOwner();
	if (cont->GetPawn()->IsA(AMyBotCharacter_2::StaticClass())) {
		auto const cont2 = Cast<AMyTestGameAIController>(owner_Comp.GetAIOwner());
		auto const npc = cont2->GetPawn();

		FVector const origin = npc->GetActorLocation();
		FNavLocation loc;

		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
		{		
			cont2->GetBlackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}
	}
	else if (cont->GetPawn()->IsA(AMyBotCharacter_3::StaticClass())) {
		auto const cont2 = Cast<AAIController_Magician>(owner_Comp.GetAIOwner());
		auto const npc = cont2->GetPawn();

		FVector const origin = npc->GetActorLocation();
		FNavLocation loc;

		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
		{
			cont2->GetBlackboard3()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}
	}
	else if (cont->GetPawn()->IsA(AMyBossCharacter::StaticClass())) {
		auto const cont2 = Cast<AMyAIBossController>(owner_Comp.GetAIOwner());
		auto const npc = cont2->GetPawn();

		FVector const origin = npc->GetActorLocation();
		FNavLocation loc;

		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
		{
			cont2->GetBlackboard2()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}

	}

    FinishLatentTask(owner_Comp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}