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
	// AI 컨트롤러와 블랙보드 컴포넌트 가져오기
	auto const cont = owner_comp.GetAIOwner();
	auto const blackboard = owner_comp.GetBlackboardComponent();

	// AI의 캐릭터 가져오기
	AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(cont->GetPawn());

	// 플레이어와 위치가져오기
	APlayerCharacter* player_character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector const player_location = player_character->GetActorLocation();
	
	//스킬 애니메이션 재생
	AICharacter->GetMesh()->GetAnimInstance()->Montage_Play(AICharacter->BossSkill3AnimMontage);


	//플레이어 위치에 3초간 데칼엑터 생성
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

	

	//플레이어 위치에 1.5초뒤에 얼음엑터가 떨어지는 코드
	// 플레이어 위치에서 300 유닛 위에 있는 위치 계산
	FVector SpawnLocation = player_location + FVector(0, 0, 500);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Actor Spawn Location : SpawnLocation %s"), *SpawnLocation.ToString()));
	// 1.5초 후에 얼음 액터를 생성하는 타이머 설정

	

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, [this, World, SpawnLocation]()
	{
			
		// 얼음 액터를 생성하고, 플레이어 위로부터 떨어지게 설정
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
