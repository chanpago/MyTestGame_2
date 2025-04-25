// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "blackboard_keys.h"
#include "BTTask_BossSkill3.h"
#include "BossSkill2_DA.h"
#include "PlayerCharacter.h"
#include "BossSill2_Actor.h"
#include "MyBotCharacter_3.h"
#include "AIController.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossSkill3::UBTTask_BossSkill3(const FObjectInitializer& object_intializer)
{
	NodeName = TEXT("Boss Skill3");
}

EBTNodeResult::Type UBTTask_BossSkill3::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AI ��Ʈ�ѷ��� ������ ������Ʈ ��������
	auto const cont = owner_comp.GetAIOwner();
	auto const blackboard = owner_comp.GetBlackboardComponent();

	// AI�� ĳ���� ��������
	AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(cont->GetPawn());

	// �÷��̾�� ��ġ��������
	APlayerCharacter* player_character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector const player_location = player_character->GetActorLocation();
	
	//��ų �ִϸ��̼� ���
	AICharacter->GetMesh()->GetAnimInstance()->Montage_Play(AICharacter->BossSkill3AnimMontage);


	//�÷��̾� ��ġ�� 3�ʰ� ��Į���� ����
	UWorld* World = GetWorld();
	if (World)
	{
		ABossSkill2_DA* SkillDecal = World->SpawnActor<ABossSkill2_DA>(ABossSkill2_DA::StaticClass(), player_location, FRotator::ZeroRotator);
		if (SkillDecal)
		{
			// Initialize the decal with a location and a duration (e.g., 3 seconds)
			SkillDecal->InitializeDecal(player_location, 1.5f);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Decal Spawn Location : Playerlocation %s"), *player_location.ToString()));
		}
	}

	

	//�÷��̾� ��ġ�� 1.5�ʵڿ� �������Ͱ� �������� �ڵ�
	// �÷��̾� ��ġ���� 300 ���� ���� �ִ� ��ġ ���
	FVector SpawnLocation = player_location + FVector(0, 0, 500);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Actor Spawn Location : SpawnLocation %s"), *SpawnLocation.ToString()));
	// 1.5�� �Ŀ� ���� ���͸� �����ϴ� Ÿ�̸� ����

	

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, [this, World, SpawnLocation]()
	{
			
		// ���� ���͸� �����ϰ�, �÷��̾� ���κ��� �������� ����
		ABossSill2_Actor* BossSkill2Actor = World->SpawnActor<ABossSill2_Actor>(SkillActorClass, SpawnLocation, FRotator::ZeroRotator);
		if (BossSkill2Actor)
		{
			BossSkill2Actor->StaticMeshComponent->SetSimulatePhysics(true);
			//BossSkill2Actor->SetActorLocation(SpawnLocation);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Ice Actor Spawn");
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Failed to Spawn Ice Actor");
		}
	}, 1.5f, false);


	

	return EBTNodeResult::Succeeded;
}

void UBTTask_BossSkill3::SpawnSkillActor(UBehaviorTreeComponent* owner_comp)
{
}
