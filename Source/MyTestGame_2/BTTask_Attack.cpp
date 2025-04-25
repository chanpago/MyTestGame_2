//game
#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "AIController.h"
#include "MyBotCharacter_3.h"
#include "MyBasicCharacter_2.h"
#include "MyBotCharacter_2.h"
#include "CombatInterface.h"

//engine
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"


UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& object_intializer)
{
	NodeName = TEXT("Melee Attack");
	
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	const AAIController* cont = owner_comp.GetAIOwner();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ExecuteTask");
	AMyBotCharacter_2* const NpcBot = Cast<AMyBotCharacter_2>(cont->GetPawn());
	ICombatInterface* icombat = Cast<ICombatInterface>(NpcBot);
	if (icombat)
	{
		if (MontageHasFinished(NpcBot))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Before Execute_MeleeAttack");
			icombat->Execute_MeleeAttack(NpcBot);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "After Execute_MeleeAttack");
		}
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}

bool UBTTask_Attack::MontageHasFinished(const AMyBotCharacter_2* NpcBot)
{
	return NpcBot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NpcBot->GetMontage());
}

bool UBTTask_Attack::MontageHasFinished(const AMyBotCharacter_3* NpcBot)
{
	return NpcBot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NpcBot->GetMontage());
}