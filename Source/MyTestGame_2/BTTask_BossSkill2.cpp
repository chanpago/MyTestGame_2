//game
#include "BTTask_BossSkill2.h"
#include "MyAIBossController.h"
#include "MyBasicCharacter_2.h"
#include "MyBossCharacter.h"
#include "MyIceHone.h"

//engine
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"

UBTTask_BossSkill2::UBTTask_BossSkill2(const FObjectInitializer& object_intializer)
{
	NodeName = TEXT("Boss Skill2");
}

EBTNodeResult::Type UBTTask_BossSkill2::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	const AAIController* Cont = owner_comp.GetAIOwner();
	if (Cont)
	{
		AActor* const Owner = Cont->GetPawn();
		if (Owner)
		{
			if (MyIceAttack) {
				const FRotator SpawnRotation = Owner->GetActorRotation();
				//const FVector SpawnLocation = Owner->GetActorLocation() + SpawnRotation.RotateVector(FVector(90.0f, 90.0f, 90.0f));

				const FVector OffsetFromOwner = FVector(1500.0f, 0.0f, 0.0f); // Adjust the offset values as needed
				const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * OffsetFromOwner.X + Owner->GetActorRightVector() * OffsetFromOwner.Y + Owner->GetActorUpVector() * OffsetFromOwner.Z;

				MyIceInstance = GetWorld()->SpawnActor<AMyIceHone>(MyIceAttack, SpawnLocation, SpawnRotation);
				if (MyIceInstance)
				{
					// 엑터가 성공적으로 생성됨
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "IceInstance ");
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "not IceInstance ");
				}
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "not myiceAttack ");
			}
		}
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
