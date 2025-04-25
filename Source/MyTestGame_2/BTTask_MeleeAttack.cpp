// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "BTTask_MeleeAttack.h"
#include "PlayerCharacter.h"
#include "blackboard_keys.h"
#include "MyBotCharacter_3.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
UBTTask_MeleeAttack::UBTTask_MeleeAttack(const FObjectInitializer& object_intialize)
{
	NodeName = TEXT("UBTTask_MeleeAttack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AI ��Ʈ�ѷ��� ������ ������Ʈ ��������
	auto const cont = owner_comp.GetAIOwner();

	// AI�� ĳ���� ��������
	AMyBotCharacter_3* const Boss = Cast<AMyBotCharacter_3>(cont->GetPawn());

	// AIController ��������
	AAIController* const ai_controller = owner_comp.GetAIOwner();

	// �÷��̾� ĳ���� ��������
	APlayerCharacter* const player_character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player_character)
	{
		// AI�� �÷��̾ �ٶ󺸰� ����
		ai_controller->SetFocus(player_character);
		UBlackboardComponent* BlackboardComp = ai_controller->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(bb_keys::IsBlockReady, true);
		}
	}

	// �ִϸ��̼� �� �ϳ� ���� ����
	UAnimMontage* SelectedAnim = nullptr;
	switch (FMath::RandRange(0, 1))  // 0, 1 �� �ϳ��� ���� ����
	{
	case 0:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Case0 Activated!");
		SelectedAnim = MeleeAttackAnim1;
		break;
	case 1:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Case1 Activated!");
		SelectedAnim = MeleeAttackAnim2;
		break;
	
	}

	// ���õ� �ִϸ��̼� ��� �� ���� �ݹ� ����
	if (SelectedAnim && Boss)
	{
		float AnimationDuration = SelectedAnim->GetPlayLength();  // �ִϸ��̼� ��� ���� ��������
		Boss->PlayAnimMontage(SelectedAnim);

		// Ÿ�̸� �����Ͽ� �ִϸ��̼� ���� �Ŀ� OnTimerFinished ȣ��
		// Ÿ�̸� ��������Ʈ ����
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UBTTask_MeleeAttack::OnTimerFinished, &owner_comp);

		// Ÿ�̸� �����Ͽ� �ִϸ��̼� ���� �Ŀ� OnTimerFinished ȣ��
		Boss->GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, AnimationDuration, false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::OnTimerFinished(UBehaviorTreeComponent* owner_comp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "OnTimerFinished!");
	// �ִϸ��̼��� ������ �� �������� �½�ũ �Ϸ� ó��
	
	if (owner_comp) {
		FinishLatentTask(*owner_comp, EBTNodeResult::Succeeded);
	}
}