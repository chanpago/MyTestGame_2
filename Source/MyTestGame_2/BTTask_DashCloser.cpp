//game
#include "PlayerCharacter.h"
#include "blackboard_keys.h"
#include "MyBotCharacter_3.h"

//engine
#include "BTTask_DashCloser.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DashCloser::UBTTask_DashCloser(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = TEXT("Dash Closer");
}

EBTNodeResult::Type UBTTask_DashCloser::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AIController ��������
	AAIController* const AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// AI�� ĳ���� ��������
		AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(AIController->GetPawn());
		if (AICharacter)
		{
			// �÷��̾��� ��ġ ��������
			ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (PlayerCharacter)
			{
				// �÷��̾� ��ġ�� AI ��ġ ���
				FVector PlayerLocation = PlayerCharacter->GetActorLocation();
				FVector AILocation = AICharacter->GetActorLocation();

				// ��� ���� ��� (�÷��̾� ��������)
				FVector DashDirection = (PlayerLocation - AILocation).GetSafeNormal();

				// ��ǥ ��ġ ��� (�÷��̾� �� 300 ���� �Ÿ�)
				FVector TargetLocation = PlayerLocation - DashDirection * 50.0f;

				// AI�� ĳ���� �����Ʈ ������Ʈ ��������
				UCharacterMovementComponent* MovementComponent = AICharacter->GetCharacterMovement();
				if (MovementComponent)
				{
					// ��� �ӵ� ����
					float DashSpeed = 4000.0f; // ���ϴ� ��� �ӵ��� ����

					// ��� �ִϸ��̼� ����
					StartDashAnimation(AICharacter);

					// ��� ����
					MovementComponent->BrakingFrictionFactor = 0.0f; // ������ ���ϱ� ���� ���� ��� ����
					MovementComponent->Velocity = DashDirection * DashSpeed;

					// ��� �� ���� ���� �� �÷��̾� �ٶ󺸱�
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AIController, PlayerCharacter, MovementComponent, AICharacter]()
						{
							MovementComponent->BrakingFrictionFactor = 2.0f; // ���� ���� ����� ����

							// ��� �ִϸ��̼� ����
							StopDashAnimation(AICharacter);

							// �÷��̾ �ٶ󺸵��� ����
							AIController->SetFocus(PlayerCharacter);

							// �������� ShortDistance ���� true�� ����
							UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
							if (BlackboardComp)
							{
								BlackboardComp->SetValueAsBool(bb_keys::ShortDistance, true);
							}
						}, 0.5f, false); // 0.5�� �� ���� (��� �ð�)

					// ���� ��ȯ
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	// ���� ��ȯ
	return EBTNodeResult::Failed;
}

void UBTTask_DashCloser::StartDashAnimation(AMyBotCharacter_3* AICharacter)
{
	if (AICharacter->GetMesh()->GetAnimInstance())
	{
		AICharacter->GetMesh()->GetAnimInstance()->Montage_Play(AICharacter->DashAnimMontage);
	}
}

void UBTTask_DashCloser::StopDashAnimation(AMyBotCharacter_3* AICharacter)
{
	if (AICharacter->GetMesh()->GetAnimInstance())
	{
		AICharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, AICharacter->DashAnimMontage);
	}
}