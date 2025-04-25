//game
#include "blackboard_keys.h"
#include "BTService_RandomNumber2.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/RandomStream.h"



void UBTService_RandomNumber2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // RandomStream ���� (�ð��� ����� �õ� ���)
    FRandomStream RandomStream(FMath::Rand());

    // 1���� 3������ ���� ���� ����
    int32 RandomValue = RandomStream.RandRange(1, 10);

    // ������ ������Ʈ�� ���� �����忡 ����
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // ������ Ű�� "RandomNumber"�� ���� ���ڸ� ����
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsInt(bb_keys::RandomNumber, RandomValue);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("RandomValue: %d"), RandomValue));
    }
}
