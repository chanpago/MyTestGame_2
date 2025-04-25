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
				const FVector SpawnCenter = Owner->GetActorLocation(); // ���� ĳ������ ��ġ
				float Radius = 1000.0f; // ù ��° ���� �ݰ�
				int32 NumInstances = 36; // ù ��° ������ ������ �ν��Ͻ��� ����

				// ù ��° �ݸ��� ����
				for (int32 i = 0; i < NumInstances; ++i)
				{
					const float Angle = i * (360.0f / NumInstances); // �� �ν��Ͻ��� ����
					const float Rad = FMath::DegreesToRadians(Angle); // ������ �������� ��ȯ

					const FVector SpawnLocation = SpawnCenter + FVector(FMath::Cos(Rad) * Radius, FMath::Sin(Rad) * Radius, 0.0f); // ���� ��ġ
					const FRotator SpawnRotation = Owner->GetActorRotation(); // ���� ĳ������ ȸ��

					AMyIceHone* IceInstance = GetWorld()->SpawnActor<AMyIceHone>(MyIceAttack, SpawnLocation, SpawnRotation);
					if (IceInstance)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Spawned IceInstance");

						// 1�� �Ŀ� IceInstance ����
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

				// �� ��° �ݸ��� ���� (1�� �Ŀ� �����ǵ��� Ÿ�̸� ����)
				FTimerHandle SecondCollisionTimerHandle;
				Owner->GetWorldTimerManager().SetTimer(SecondCollisionTimerHandle, FTimerDelegate::CreateLambda([this, Owner, SpawnCenter]()
					{
						float Radius2 = 1500.0f; // �� ��° ���� �ݰ�
						int32 NumInstances2 = 100; // �� ��° ������ ������ �ν��Ͻ��� ����

						for (int32 i = 0; i < NumInstances2; ++i)
						{
							const float Angle = i * (360.0f / NumInstances2); // �� �ν��Ͻ��� ����
							const float Rad = FMath::DegreesToRadians(Angle); // ������ �������� ��ȯ

							const FVector SpawnLocation = SpawnCenter + FVector(FMath::Cos(Rad) * Radius2, FMath::Sin(Rad) * Radius2, 0.0f); // ���� ��ġ
							const FRotator SpawnRotation = Owner->GetActorRotation(); // ���� ĳ������ ȸ��

							AMyIceHone* IceInstance = Owner->GetWorld()->SpawnActor<AMyIceHone>(MyIceAttack, SpawnLocation, SpawnRotation);
							if (IceInstance)
							{
								//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Spawned Second IceInstance");

								// 1�� �Ŀ� IceInstance ����
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
	// ���� �ʿ�
}

bool UBTTask_BossSkill1::MontageHasFinished(const AMyBossCharacter* Boss)
{
	return Boss->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Boss->GetMontage());
}