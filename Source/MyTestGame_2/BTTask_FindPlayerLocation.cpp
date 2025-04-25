#pragma once

//game
#include "BTTask_FindPlayerLocation.h"
#include "MyTestGame_2.h"
#include "AIController_Magician.h"
#include "MyAIController.h"
#include "MyAIBossController.h"
#include "MyBotCharacter_2.h"
#include "MyBossCharacter.h"
#include "MyBotCharacter_3.h"
#include "blackboard_keys.h"

//engine
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
    
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    //const FVector& PlayerLocation = Player->GetActorLocation();
   
    auto const cont = owner_comp.GetAIOwner();
    if (cont->GetPawn()->IsA(AMyBotCharacter_2::StaticClass())) {
        auto const cont2 = Cast<AMyTestGameAIController>(owner_comp.GetAIOwner());
        FVector const PlayerLocation = Player->GetActorLocation();
        FNavLocation loc;
        if (search_random)
        {
            FNavLocation OutNavLocation;
            UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
            if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, search_radius, OutNavLocation, nullptr))
            {
                cont2->GetBlackboard()->SetValueAsVector(bb_keys::target_location, OutNavLocation.Location);           
            }
        }
        else
        {
            cont2->GetBlackboard()->SetValueAsVector(bb_keys::target_location, PlayerLocation);
        }
    }
    else if (cont->GetPawn()->IsA(AMyBotCharacter_3::StaticClass())) {
        auto const cont2 = Cast<AAIController_Magician>(owner_comp.GetAIOwner());
        FVector const PlayerLocation = Player->GetActorLocation();
        FNavLocation loc;

        if (search_random)
        {
            FNavLocation OutNavLocation;
            UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
            if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, search_radius, OutNavLocation, nullptr))
            {
                cont2->GetBlackboard3()->SetValueAsVector(bb_keys::target_location, OutNavLocation.Location);
            }
        }
        else
        {
            cont2->GetBlackboard3()->SetValueAsVector(bb_keys::target_location, PlayerLocation);
        }
    }
    else if (cont->GetPawn()->IsA(AMyBossCharacter::StaticClass())) {
        auto const cont2 = Cast<AMyAIBossController>(owner_comp.GetAIOwner());

        FVector const PlayerLocation = Player->GetActorLocation();
        FNavLocation loc;

        if (search_random)
        {
            FNavLocation OutNavLocation;

            const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
            if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, search_radius, OutNavLocation, nullptr))
            {
                cont2->GetBlackboard2()->SetValueAsVector(bb_keys::target_location, OutNavLocation.Location);
                // get_blackboard -> GetBlackBoard() 로 바꾸는게 좋음 (UpperCamelCase식 명칭이 요새 트렌드 ㅋ)
            }
        }
        else
        {
            cont2->GetBlackboard2()->SetValueAsVector(bb_keys::target_location, PlayerLocation);
        }
    }
    FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}