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
	// AIController 가져오기
	AAIController* const AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// AI의 캐릭터 가져오기
		AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(AIController->GetPawn());
		if (AICharacter)
		{
			// 플레이어의 위치 가져오기
			ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (PlayerCharacter)
			{
				// 플레이어 위치와 AI 위치 계산
				FVector PlayerLocation = PlayerCharacter->GetActorLocation();
				FVector AILocation = AICharacter->GetActorLocation();

				// 대시 벡터 계산 (플레이어 방향으로)
				FVector DashDirection = (PlayerLocation - AILocation).GetSafeNormal();

				// 목표 위치 계산 (플레이어 앞 300 유닛 거리)
				FVector TargetLocation = PlayerLocation - DashDirection * 50.0f;

				// AI의 캐릭터 무브먼트 컴포넌트 가져오기
				UCharacterMovementComponent* MovementComponent = AICharacter->GetCharacterMovement();
				if (MovementComponent)
				{
					// 대시 속도 설정
					float DashSpeed = 4000.0f; // 원하는 대시 속도로 설정

					// 대시 애니메이션 시작
					StartDashAnimation(AICharacter);

					// 대시 시작
					MovementComponent->BrakingFrictionFactor = 0.0f; // 감속을 피하기 위해 마찰 계수 설정
					MovementComponent->Velocity = DashDirection * DashSpeed;

					// 대시 후 감속 복구 및 플레이어 바라보기
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AIController, PlayerCharacter, MovementComponent, AICharacter]()
						{
							MovementComponent->BrakingFrictionFactor = 2.0f; // 원래 마찰 계수로 복구

							// 대시 애니메이션 멈춤
							StopDashAnimation(AICharacter);

							// 플레이어를 바라보도록 설정
							AIController->SetFocus(PlayerCharacter);

							// 블랙보드의 ShortDistance 값을 true로 설정
							UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
							if (BlackboardComp)
							{
								BlackboardComp->SetValueAsBool(bb_keys::ShortDistance, true);
							}
						}, 0.5f, false); // 0.5초 후 복구 (대시 시간)

					// 성공 반환
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	// 실패 반환
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