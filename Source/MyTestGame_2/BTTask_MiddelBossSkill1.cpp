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

	// ������ Ű ����
	SkillActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MiddelBossSkill1, SkillActorKey), AMiddleSkill1IceActor::StaticClass());
}

EBTNodeResult::Type UBTTask_MiddelBossSkill1::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	// AI ��Ʈ�ѷ��� ������ ������Ʈ ��������
	auto const cont = owner_comp.GetAIOwner();
	auto const blackboard = owner_comp.GetBlackboardComponent();

	// ������ Ű ����
	blackboard->SetValueAsBool(bb_keys::notchaseplayer, true);

	// �̹� �����ϴ� ��ų ���Ͱ� �ִ��� Ȯ��
	auto* existing_skill_actor = Cast<AMiddleSkill1IceActor>(blackboard->GetValueAsObject(SkillActorKey.SelectedKeyName));
	if (existing_skill_actor)
	{
		// �̹� ��ų ���Ͱ� �����ϸ� �۾��� �Ϸ�� ����
		FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Ÿ�̸� ����
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_MiddelBossSkill1::SpawnSkillActor, &owner_comp), 1.0f, false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MiddelBossSkill1::SpawnSkillActor(UBehaviorTreeComponent* owner_comp)
{
	// AI ��Ʈ�ѷ��� ������ ������Ʈ ��������
	auto const cont = owner_comp->GetAIOwner();
	auto const blackboard = owner_comp->GetBlackboardComponent();

	// �÷��̾� ��ġ ��������
	FVector const player_location = blackboard->GetValueAsVector(bb_keys::target_location);

	// MiddleSkill1IceActor ���� ��ġ ����
	FVector const spawn_location = cont->GetPawn()->GetActorLocation();
	FRotator const spawn_rotation = (player_location - spawn_location).Rotation();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("SpawnSkillActor Set"));

	UWorld* world = GetWorld();
	if (world && SkillActorClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("World && SkillActorClass Set"));
		// MiddleSkill1IceActor ����
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = cont->GetPawn();
		auto* skill_actor = world->SpawnActor<AMiddleSkill1IceActor>(SkillActorClass, spawn_location, spawn_rotation, spawn_params);

		if (skill_actor)
		{
			// ��ų�� �ӵ� ����
			if (auto* movement_comp = skill_actor->ProjectileMovement)
			{
				FVector const direction = (player_location - spawn_location).GetSafeNormal();
				movement_comp->Velocity = direction * movement_comp->InitialSpeed;
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Skill Actor Velocity Set"));
			}

			// ����� �޽���
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