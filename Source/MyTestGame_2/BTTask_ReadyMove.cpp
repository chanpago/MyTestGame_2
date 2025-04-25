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
	// AIController ��������
	AAIController* const ai_controller = owner_comp.GetAIOwner();
	// AI�� ĳ���� ��������
	AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());

	// ������ ���� ���� (���� �Ǵ� ������)
	FVector move_direction = FMath::RandBool() ? FVector::RightVector : FVector::LeftVector;

	// �̵� ���� ���
	FVector move_location = ai_character->GetActorLocation() + move_direction * 400.0f; // 200 ���� �̵�

	// AIController�� MoveToLocation ���
	ai_controller->MoveToLocation(move_location);


	return EBTNodeResult::Succeeded;
}
