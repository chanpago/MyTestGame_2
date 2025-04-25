//game
#include "BTTask_ChasePlayer.h"
#include "MyAIController.h"
#include "MyAIBossController.h"
#include "AIController_Magician.h"
#include "blackboard_keys.h"
#include "MyBotCharacter_3.h"
#include "MyBotCharacter_2.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"



UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = owner_comp.GetAIOwner();
	auto const blackboard = cont->GetBlackboardComponent();

	// 블랙보드 키 설정
	blackboard->SetValueAsBool(bb_keys::should_play_animation, true);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetValueAsBool should_play_animation");
	if (cont->GetPawn()->IsA(AMyBotCharacter_2::StaticClass())) {
		AMyTestGameAIController* const cont2 = Cast<AMyTestGameAIController>(owner_comp.GetAIOwner());
		FVector const player_location = cont2->GetBlackboard()->GetValueAsVector(bb_keys::target_location);

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont2, player_location);

	}
	else if (cont->GetPawn()->IsA(AMyBotCharacter_3::StaticClass())) {
		AAIController_Magician* const cont2 = Cast<AAIController_Magician>(owner_comp.GetAIOwner());
		FVector const player_location = cont2->GetBlackboard3()->GetValueAsVector(bb_keys::target_location);

		AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(cont2->GetPawn());
		AICharacter->GetMesh()->GetAnimInstance()->Montage_Play(AICharacter->DashAnimMontage);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont2, player_location);

	}
	else {
		AMyAIBossController* const cont2 = Cast<AMyAIBossController>(owner_comp.GetAIOwner());
		FVector const player_location = cont2->GetBlackboard2()->GetValueAsVector(bb_keys::target_location);

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont2, player_location);
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}