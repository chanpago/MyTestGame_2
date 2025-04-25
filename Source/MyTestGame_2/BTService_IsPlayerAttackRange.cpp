//game
#include "BTService_IsPlayerAttackRange.h"
#include "MyBotCharacter_2.h"
#include "MyBotCharacter_3.h"
#include "MyBossCharacter.h"
#include "PlayerCharacter.h"
#include "MyAIController.h"
#include "AIController_Magician.h"
#include "MyAIBossController.h"
#include "blackboard_keys.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerAttackRange::UBTService_IsPlayerAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Attack Range");
}

void UBTService_IsPlayerAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	AttackRange = 1200.0f;

	auto const AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	auto const AICharacter = AIController->GetPawn();
	if (!AICharacter) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	bool bIsInRange = false;

	if (AICharacter->IsA(AMyBotCharacter_2::StaticClass()))
	{
		AMyBotCharacter_2* Bot = Cast<AMyBotCharacter_2>(AICharacter);
		bIsInRange = Bot->GetDistanceTo(Player) <= AttackRange;
	}
	else if (AICharacter->IsA(AMyBotCharacter_3::StaticClass()))
	{
		AMyBotCharacter_3* Bot = Cast<AMyBotCharacter_3>(AICharacter);
		bIsInRange = Bot->GetDistanceTo(Player) <= AttackRange;
	}
	else if (AICharacter->IsA(AMyBossCharacter::StaticClass()))
	{
		AMyBossCharacter* Boss = Cast<AMyBossCharacter>(AICharacter);
		bIsInRange = Boss->GetDistanceTo(Player) <= AttackRange;
	}

	if (bIsInRange)
	{
		if (AICharacter->IsA(AMyBotCharacter_2::StaticClass()))
		{
			AMyTestGameAIController* TestController = Cast<AMyTestGameAIController>(AIController);
			TestController->GetBlackboard()->SetValueAsBool(bb_keys::PlayerIsAttackRange, true);
		}
		else if (AICharacter->IsA(AMyBotCharacter_3::StaticClass()))
		{
			AAIController_Magician* MagicianController = Cast<AAIController_Magician>(AIController);
			MagicianController->GetBlackboard3()->SetValueAsBool(bb_keys::PlayerIsAttackRange, true);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AttackRange <= 1200");
		}
		else if (AICharacter->IsA(AMyBossCharacter::StaticClass()))
		{
			AMyAIBossController* BossController = Cast<AMyAIBossController>(AIController);
			BossController->GetBlackboard2()->SetValueAsBool(bb_keys::PlayerIsAttackRange, true);
		}
	}
	else
	{
		if (AICharacter->IsA(AMyBotCharacter_2::StaticClass()))
		{
			AMyTestGameAIController* TestController = Cast<AMyTestGameAIController>(AIController);
			TestController->GetBlackboard()->SetValueAsBool(bb_keys::PlayerIsAttackRange, false);
		}
		else if (AICharacter->IsA(AMyBotCharacter_3::StaticClass()))
		{
			AAIController_Magician* MagicianController = Cast<AAIController_Magician>(AIController);
			MagicianController->GetBlackboard3()->SetValueAsBool(bb_keys::PlayerIsAttackRange, false);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AttackRange >= 1200");
		}
		else if (AICharacter->IsA(AMyBossCharacter::StaticClass()))
		{
			AMyAIBossController* BossController = Cast<AMyAIBossController>(AIController);
			BossController->GetBlackboard2()->SetValueAsBool(bb_keys::PlayerIsAttackRange, false);
		}
	}
}