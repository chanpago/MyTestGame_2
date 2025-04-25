//game
#include "blackboard_keys.h"
#include "BTService_RandomNumber.h"

//engine
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/RandomStream.h"


void UBTService_RandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 랜덤 정수 생성
    int32 MinValue = 1;
    int32 MaxValue = 2;
    int32 RandomValue = FMath::RandRange(MinValue, MaxValue);

    // Blackboard에 저장
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsInt(GetSelectedBlackboardKey(), RandomValue);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RandomValue: %d"), RandomValue));
    }
    else {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No BlackboardComp");
    }
}
