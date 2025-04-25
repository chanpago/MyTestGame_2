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
    // AIController ��������
    AAIController* const ai_controller = owner_comp.GetAIOwner();
    if (ai_controller)
    {
        // AI�� ĳ���� ��������
        AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());
        if (ai_character)
        {
            // �÷��̾��� ��ġ ��������
            ACharacter* const player_character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (player_character)
            {
                // AI ĳ������ ���� ��ġ
                FVector ai_location = ai_character->GetActorLocation();

                // �׺���̼� �ý��� ��������
                UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
                if (NavSystem)
                {
                    // FNavLocation ������ ����Ͽ� ���� ��ġ ����
                    FNavLocation RandomLocation;
                    if (NavSystem->GetRandomReachablePointInRadius(ai_location, 600.0f, RandomLocation))
                    {
                        // ��� �ִϸ��̼� ����
                        StartDashAnimation(ai_character);

                        // AI�� ���� ��ġ�� �̵���Ű��
                        FAIRequestID RequestID = ai_controller->MoveToLocation(RandomLocation.Location, -1, true, true, false, true, 0, true);

                        // �̵� �Ϸ� ��������Ʈ ����
                        FTimerDelegate TimerDel;
                        TimerDel.BindUObject(this, &UBTTask_DashAway::OnMoveCompleted, ai_controller, ai_character, RequestID);

                        FTimerHandle TimerHandle;
                        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);

                        // �������� ShortDistance ���� false�� ����
                        UBlackboardComponent* BlackboardComp = ai_controller->GetBlackboardComponent();
                        if (BlackboardComp)
                        {
                            BlackboardComp->SetValueAsBool(bb_keys::ShortDistance, false);
                        }

                        // ���� ��ȯ
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
        }
    }

    // ���� ��ȯ
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
        // �̵��� �Ϸ�Ǿ��� �� �ִϸ��̼� ����
        StopDashAnimation(ai_character);
    }
}