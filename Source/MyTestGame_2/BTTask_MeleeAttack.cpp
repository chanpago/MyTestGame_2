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
	// AI 컨트롤러와 블랙보드 컴포넌트 가져오기
	auto const cont = owner_comp.GetAIOwner();

	// AI의 캐릭터 가져오기
	AMyBotCharacter_3* const Boss = Cast<AMyBotCharacter_3>(cont->GetPawn());

	// AIController 가져오기
	AAIController* const ai_controller = owner_comp.GetAIOwner();

	// 플레이어 캐릭터 가져오기
	APlayerCharacter* const player_character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player_character)
	{
		// AI가 플레이어를 바라보게 설정
		ai_controller->SetFocus(player_character);
		UBlackboardComponent* BlackboardComp = ai_controller->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(bb_keys::IsBlockReady, true);
		}
	}

	// 애니메이션 중 하나 랜덤 선택
	UAnimMontage* SelectedAnim = nullptr;
	switch (FMath::RandRange(0, 1))  // 0, 1 중 하나를 랜덤 선택
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

	// 선택된 애니메이션 재생 및 종료 콜백 설정
	if (SelectedAnim && Boss)
	{
		float AnimationDuration = SelectedAnim->GetPlayLength();  // 애니메이션 재생 길이 가져오기
		Boss->PlayAnimMontage(SelectedAnim);

		// 타이머 설정하여 애니메이션 길이 후에 OnTimerFinished 호출
		// 타이머 델리게이트 생성
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UBTTask_MeleeAttack::OnTimerFinished, &owner_comp);

		// 타이머 설정하여 애니메이션 길이 후에 OnTimerFinished 호출
		Boss->GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, AnimationDuration, false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::OnTimerFinished(UBehaviorTreeComponent* owner_comp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "OnTimerFinished!");
	// 애니메이션이 끝났을 때 성공으로 태스크 완료 처리
	
	if (owner_comp) {
		FinishLatentTask(*owner_comp, EBTNodeResult::Succeeded);
	}
}