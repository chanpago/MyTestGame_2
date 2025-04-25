//game
#include "MyIceHone.h"
#include "MyAIBossController.h"
#include "MyBasicCharacter_2.h"
#include "MyBossCharacter.h"

//engine
#include "BTTask_BossSkill1.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "CombatInterfaceBoss.h"
#include "Engine.h"

UBTTask_BossSkill1::UBTTask_BossSkill1(FObjectInitializer const& object_intializer)
{
	NodeName = TEXT("Boss Skill1");
}

EBTNodeResult::Type UBTTask_BossSkill1::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	const AAIController* Cont = owner_comp.GetAIOwner();
	if (Cont)
	{
		AActor* const Owner = Cont->GetPawn();
		if (Owner)
		{
			if (MyIceAttack)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "MyIceAttack is set");
				const FVector SpawnCenter = Owner->GetActorLocation(); // 보스 캐릭터의 위치
				float Radius = 1000.0f; // 첫 번째 원의 반경
				int32 NumInstances = 36; // 첫 번째 원에서 생성할 인스턴스의 개수

				// 첫 번째 콜리전 생성
				for (int32 i = 0; i < NumInstances; ++i)
				{
					const float Angle = i * (360.0f / NumInstances); // 각 인스턴스의 각도
					const float Rad = FMath::DegreesToRadians(Angle); // 각도를 라디안으로 변환

					const FVector SpawnLocation = SpawnCenter + FVector(FMath::Cos(Rad) * Radius, FMath::Sin(Rad) * Radius, 0.0f); // 원형 배치
					const FRotator SpawnRotation = Owner->GetActorRotation(); // 보스 캐릭터의 회전

					AMyIceHone* IceInstance = GetWorld()->SpawnActor<AMyIceHone>(MyIceAttack, SpawnLocation, SpawnRotation);
					if (IceInstance)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Spawned IceInstance");

						// 1초 후에 IceInstance 제거
						FTimerHandle TimerHandle;
						Owner->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([IceInstance]()
							{
								if (IceInstance)
								{
									IceInstance->Destroy();
								}
							}), 1.0f, false);
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Failed to spawn IceInstance");
					}
				}

				// 두 번째 콜리전 생성 (1초 후에 생성되도록 타이머 설정)
				FTimerHandle SecondCollisionTimerHandle;
				Owner->GetWorldTimerManager().SetTimer(SecondCollisionTimerHandle, FTimerDelegate::CreateLambda([this, Owner, SpawnCenter]()
					{
						float Radius2 = 1500.0f; // 두 번째 원의 반경
						int32 NumInstances2 = 100; // 두 번째 원에서 생성할 인스턴스의 개수

						for (int32 i = 0; i < NumInstances2; ++i)
						{
							const float Angle = i * (360.0f / NumInstances2); // 각 인스턴스의 각도
							const float Rad = FMath::DegreesToRadians(Angle); // 각도를 라디안으로 변환

							const FVector SpawnLocation = SpawnCenter + FVector(FMath::Cos(Rad) * Radius2, FMath::Sin(Rad) * Radius2, 0.0f); // 원형 배치
							const FRotator SpawnRotation = Owner->GetActorRotation(); // 보스 캐릭터의 회전

							AMyIceHone* IceInstance = Owner->GetWorld()->SpawnActor<AMyIceHone>(MyIceAttack, SpawnLocation, SpawnRotation);
							if (IceInstance)
							{
								//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Spawned Second IceInstance");

								// 1초 후에 IceInstance 제거
								FTimerHandle TimerHandle;
								Owner->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([IceInstance]()
									{
										if (IceInstance)
										{
											IceInstance->Destroy();
										}
									}), 1.0f, false);
							}
							else
							{
								//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Failed to spawn Second IceInstance");
							}
						}
					}), 1.0f, false);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "MyIceAttack is null");
			}
		}
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

void UBTTask_BossSkill1::DelayedTask()
{
	// 구현 필요
}

bool UBTTask_BossSkill1::MontageHasFinished(const AMyBossCharacter* Boss)
{
	return Boss->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Boss->GetMontage());
}