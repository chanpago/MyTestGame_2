
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
	// AIController ��������
	AAIController* const ai_controller = owner_Comp.GetAIOwner();
	if (ai_controller)
	{
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
	}

	FinishLatentTask(owner_Comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

