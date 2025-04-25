//game
#include "BTTask_MiddelBossSkill1.h"
#include "MiddleSkill1IceActor.h"
#include "blackboard_keys.h"
#include "MyBotCharacter_3.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController_Magician.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"


UBTTask_MiddelBossSkill1::UBTTask_MiddelBossSkill1(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Middle Boss Skill 1");

	// 블랙보드 키 설정
	SkillActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MiddelBossSkill1, SkillActorKey), AMiddleSkill1IceActor::StaticClass());
}

EBTNodeResult::Type UBTTask_MiddelBossSkill1::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	// AI 컨트롤러와 블랙보드 컴포넌트 가져오기
	auto const cont = owner_comp.GetAIOwner();
	auto const blackboard = owner_comp.GetBlackboardComponent();

	// 블랙보드 키 설정
	blackboard->SetValueAsBool(bb_keys::notchaseplayer, true);

	// 이미 존재하는 스킬 액터가 있는지 확인
	auto* existing_skill_actor = Cast<AMiddleSkill1IceActor>(blackboard->GetValueAsObject(SkillActorKey.SelectedKeyName));
	if (existing_skill_actor)
	{
		// 이미 스킬 액터가 존재하면 작업을 완료로 설정
		FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// 타이머 설정
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_MiddelBossSkill1::SpawnSkillActor, &owner_comp), 1.0f, false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MiddelBossSkill1::SpawnSkillActor(UBehaviorTreeComponent* owner_comp)
{
	// AI 컨트롤러와 블랙보드 컴포넌트 가져오기
	auto const cont = owner_comp->GetAIOwner();
	auto const blackboard = owner_comp->GetBlackboardComponent();

	// 플레이어 위치 가져오기
	FVector const player_location = blackboard->GetValueAsVector(bb_keys::target_location);

	// MiddleSkill1IceActor 스폰 위치 설정
	FVector const spawn_location = cont->GetPawn()->GetActorLocation();
	FRotator const spawn_rotation = (player_location - spawn_location).Rotation();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("SpawnSkillActor Set"));

	UWorld* world = GetWorld();
	if (world && SkillActorClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("World && SkillActorClass Set"));
		// MiddleSkill1IceActor 생성
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = cont->GetPawn();
		auto* skill_actor = world->SpawnActor<AMiddleSkill1IceActor>(SkillActorClass, spawn_location, spawn_rotation, spawn_params);

		if (skill_actor)
		{
			// 스킬의 속도 설정
			if (auto* movement_comp = skill_actor->ProjectileMovement)
			{
				FVector const direction = (player_location - spawn_location).GetSafeNormal();
				movement_comp->Velocity = direction * movement_comp->InitialSpeed;
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Skill Actor Velocity Set"));
			}

			// 디버그 메시지
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Skill Actor Spawned"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Failed to Spawn Skill Actor"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("World && SkillActorClass not Set"));
	}

	FinishLatentTask(*owner_comp, EBTNodeResult::Succeeded);
}