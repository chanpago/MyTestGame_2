 // game
#include "PlayerCharacter.h"
#include "blackboard_keys.h"
#include "MyBotCharacter_3.h"

// engine
#include "BTTask_DashAway.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DashAway::UBTTask_DashAway(FObjectInitializer const& object_initializer)
    : Super(object_initializer)
{
    NodeName = TEXT("Dash Away");
}

EBTNodeResult::Type UBTTask_DashAway::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    // AIController 가져오기
    AAIController* const ai_controller = owner_comp.GetAIOwner();
    if (ai_controller)
    {
        // AI의 캐릭터 가져오기
        AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());
        if (ai_character)
        {
            // 플레이어의 위치 가져오기
            ACharacter* const player_character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (player_character)
            {
                // AI 캐릭터의 현재 위치
                FVector ai_location = ai_character->GetActorLocation();

                // 네비게이션 시스템 가져오기
                UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
                if (NavSystem)
                {
                    // FNavLocation 변수를 사용하여 랜덤 위치 생성
                    FNavLocation RandomLocation;
                    if (NavSystem->GetRandomReachablePointInRadius(ai_location, 600.0f, RandomLocation))
                    {
                        // 대시 애니메이션 시작
                        StartDashAnimation(ai_character);

                        // AI를 랜덤 위치로 이동시키기
                        FAIRequestID RequestID = ai_controller->MoveToLocation(RandomLocation.Location, -1, true, true, false, true, 0, true);

                        // 이동 완료 델리게이트 설정
                        FTimerDelegate TimerDel;
                        TimerDel.BindUObject(this, &UBTTask_DashAway::OnMoveCompleted, ai_controller, ai_character, RequestID);

                        FTimerHandle TimerHandle;
                        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);

                        // 블랙보드의 ShortDistance 값을 false로 설정
                        UBlackboardComponent* BlackboardComp = ai_controller->GetBlackboardComponent();
                        if (BlackboardComp)
                        {
                            BlackboardComp->SetValueAsBool(bb_keys::ShortDistance, false);
                        }

                        // 성공 반환
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
        }
    }

    // 실패 반환
    return EBTNodeResult::Failed;
}

void UBTTask_DashAway::StartDashAnimation(AMyBotCharacter_3* AICharacter)
{
    if (AICharacter->GetMesh()->GetAnimInstance())
    {
        AICharacter->GetMesh()->GetAnimInstance()->Montage_Play(AICharacter->DashAnimMontage);
    }
}

void UBTTask_DashAway::StopDashAnimation(AMyBotCharacter_3* AICharacter)
{
    if (AICharacter->GetMesh()->GetAnimInstance())
    {
        AICharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, AICharacter->DashAnimMontage);
    }
}

void UBTTask_DashAway::OnMoveCompleted(AAIController* ai_controller, AMyBotCharacter_3* ai_character, FAIRequestID RequestID)
{
    if (ai_controller->GetCurrentMoveRequestID() != RequestID)
    {
        // 이동이 완료되었을 때 애니메이션 중지
        StopDashAnimation(ai_character);
    }
}