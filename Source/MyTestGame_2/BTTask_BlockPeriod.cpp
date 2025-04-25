//game
#include "PlayerCharacter.h"
#include "MyBotCharacter_3.h"
#include "blackboard_keys.h"
#include "BTTask_BlockPeriod.h"
#include "MyTestWeapon.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UBTTask_BlockPeriod::UBTTask_BlockPeriod(FObjectInitializer const& object_initializer)
	: Super(object_initializer)
{
	NodeName = TEXT("Block Period");
	BlockDuration = 2.0f; // ���ϴ� ��� ���� �ð� (�� ����)
}

EBTNodeResult::Type UBTTask_BlockPeriod::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AIController ��������
	AAIController* const ai_controller = owner_comp.GetAIOwner();
	// AI�� ĳ���� ��������
	AMyBotCharacter_3* const ai_character = Cast<AMyBotCharacter_3>(ai_controller->GetPawn());

	// �÷��̾� ĳ���� ��������
	APlayerCharacter* const player_character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(ai_controller->GetWorld(), 0));
	PlayerCharacterInstance = player_character;
	if (player_character->IsDuringAttack)
	{
		if (player_character->GetFirstWeapon()->BlockAvailable)
		{
			//MoveToSide(ai_controller, ai_character, player_character);
			StartBlock(ai_character);
			// Ÿ�̸� ����
			GetWorld()->GetTimerManager().SetTimer(BlockTimerHandle, this, &UBTTask_BlockPeriod::FinishBlock, BlockDuration, false);
			
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "EBTNodeResult::Succeeded");
	return EBTNodeResult::Succeeded;
}

void UBTTask_BlockPeriod::MoveToSide(AAIController* ai_controller, ACharacter* ai_character, ACharacter* player_character)
{
	// ������ ���� ���� (���� �Ǵ� ������)
	FVector move_direction = FMath::RandBool() ? FVector::RightVector : FVector::LeftVector;

	// �̵� ���� ���
	FVector move_location = ai_character->GetActorLocation() + move_direction * 200.0f; // 200 ���� �̵�

	// AIController�� MoveToLocation ���
	ai_controller->MoveToLocation(move_location);
}

void UBTTask_BlockPeriod::StartBlock(AMyBotCharacter_3* ai_character)
{
	// ��� �ִϸ��̼� ��Ÿ�� ���
	if (ai_character->GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = ai_character->GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(ai_character->BlockAnimMontage);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BlockAnimMontage!"));

		// �ִϸ��̼� ��Ÿ�ְ� ������ �� ȣ��� ��������Ʈ ���ε�
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UBTTask_BlockPeriod::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ai_character->BlockAnimMontage);

		PlayerCharacterInstance->GetFirstWeapon()->BlockAvailable = false;
	}
}

void UBTTask_BlockPeriod::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->IsValidLowLevel() && !bInterrupted && BlackboardCompInstance)
	{
		BlackboardCompInstance->SetValueAsBool(bb_keys::IsBlockReady, true);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BlockAnimMontage Ended!"));
	}
}

void UBTTask_BlockPeriod::FinishBlock()
{
	if (OwnerCompInstance)
	{
		AAIController* const AIController = OwnerCompInstance->GetAIOwner();
		if (AIController)
		{
			AMyBotCharacter_3* const AICharacter = Cast<AMyBotCharacter_3>(AIController->GetPawn());
			if (AICharacter && AICharacter->GetMesh()->GetAnimInstance())
			{
				AICharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, AICharacter->BlockAnimMontage);

				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				if (BlackboardComp)
				{
					BlackboardComp->SetValueAsBool(bb_keys::IsBlockReady, false);
				}
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "*OwnerCompInstance, EBTNodeResult::Succeeded");
		FinishLatentTask(*OwnerCompInstance, EBTNodeResult::Succeeded);
	}
}