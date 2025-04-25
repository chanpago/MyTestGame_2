
//game
#include "MyBTTask_BossFocus.h"
#include "PlayerCharacter.h"
#include "blackboard_keys.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UMyBTTask_BossFocus::UMyBTTask_BossFocus(FObjectInitializer const& object_initializer)
	: Super(object_initializer)
{
	NodeName = TEXT("Boss Focus");
}


EBTNodeResult::Type UMyBTTask_BossFocus::ExecuteTask(UBehaviorTreeComponent& owner_Comp, uint8* NodeMemory)
{
	// AIController 가져오기
	AAIController* const ai_controller = owner_Comp.GetAIOwner();
	if (ai_controller)
	{
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
	}

	FinishLatentTask(owner_Comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

