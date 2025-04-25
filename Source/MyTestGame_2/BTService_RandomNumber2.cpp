//game
#include "blackboard_keys.h"
#include "BTService_RandomNumber2.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/RandomStream.h"



void UBTService_RandomNumber2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // RandomStream 생성 (시간에 기반한 시드 사용)
    FRandomStream RandomStream(FMath::Rand());

    // 1에서 3까지의 랜덤 숫자 생성
    int32 RandomValue = RandomStream.RandRange(1, 10);

    // 블랙보드 컴포넌트를 통해 블랙보드에 접근
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // 블랙보드 키인 "RandomNumber"에 랜덤 숫자를 설정
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsInt(bb_keys::RandomNumber, RandomValue);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("RandomValue: %d"), RandomValue));
    }
}
