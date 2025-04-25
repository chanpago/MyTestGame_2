//game
#include "Portal1.h"
#include "MyTestCharacter_2.h"
#include "PlayerCharacter.h"

//engine
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// �⺻ ���� ����
APortal1::APortal1()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal1::OnOverlapBegin);

}

void APortal1::BeginPlay()
{
    Super::BeginPlay();

}

void APortal1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APortal1::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnOverlapBegin!");
    if (OtherActor && (OtherActor != this))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OtherActor && (OtherActor != this)!");
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
        if (PlayerCharacter)
        {
            PlayerCharacter->Save();
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "PlayerCharacter!");

            if (MapSelectWidgetClass)
            {
                // ������ �����ϰ� ȭ�鿡 ���ϴ�.
                MapSelectWidget = CreateWidget<UMapSelect_Widget>(GetWorld(), MapSelectWidgetClass);
                if (MapSelectWidget)
                {
                    MapSelectWidget->AddToViewport();
                    // �Է� ��带 UI�� �����մϴ�.
                    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
                    if (PlayerController)
                    {
                        FInputModeUIOnly InputMode;
                        InputMode.SetWidgetToFocus(MapSelectWidget->TakeWidget());
                        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                        PlayerController->SetInputMode(InputMode);
                        PlayerController->bShowMouseCursor = true;
                    }
                }
            }
        }
    }
}


