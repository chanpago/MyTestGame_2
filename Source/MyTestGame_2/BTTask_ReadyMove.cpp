// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "BTTask_ReadyMove.h"
#include "MyBotCharacter_3.h"


//engine
#include "AIController.h"

UBTTask_ReadyMove::UBTTask_ReadyMove(FObjectInitializer const& object_initializer)
{
}

EBTNodeResult::Type UBTTask_ReadyMove::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AIController 가져오기
	AAIController* const ai_controller = owner_comp.GetAIOwner();
	// AI의 캐릭터 가져오기
	AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());

	// 무작위 방향 선택 (왼쪽 또는 오른쪽)
	FVector move_direction = FMath::RandBool() ? FVector::RightVector : FVector::LeftVector;

	// 이동 벡터 계산
	FVector move_location = ai_character->GetActorLocation() + move_direction * 400.0f; // 200 유닛 이동

	// AIController의 MoveToLocation 사용
	ai_controller->MoveToLocation(move_location);


	return EBTNodeResult::Succeeded;
}
