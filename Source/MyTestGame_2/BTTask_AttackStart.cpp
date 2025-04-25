//game
#include "BTTask_AttackStart.h"
#include "AIController_Magician.h"
#include "MyBotCharacter_3.h"

//engine


UBTTask_AttackStart::UBTTask_AttackStart(const FObjectInitializer& object_intializer)
{
}

EBTNodeResult::Type UBTTask_AttackStart::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAIController* const ai_controller = owner_comp.GetAIOwner();
	AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());

	ai_character->IsDuringAttack = true;

	return EBTNodeResult::Succeeded;
}
