//game
#include "BossTriggetActor.h"
#include "PlayerCharacter.h"
#include "MainHUD.h"
#include "MyBotCharacter_3.h"

//engine
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABossTriggetActor::ABossTriggetActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossTriggetActor::OnOverlapBegin);

	// Initialize the flag
	bHasPlayedMusic = false;


	
}

void ABossTriggetActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player and music has not been played yet
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter && !bHasPlayedMusic)
	{
		// Display a debug message on screen
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player entered the boss area!"));

		// Play the boss entrance music
		UGameplayStatics::PlaySound2D(GetWorld(), BossEntranceMusic);

		// Set the flag to true to prevent further music playback
		bHasPlayedMusic = true;
	}
	/*
	MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MainHUD && MainHUD->BossWidget)
	{
		MainHUD->DisPlayBossUI();
	}
	*/
	// Find the boss character
	AMyBotCharacter_3* BossCharacter = Cast<AMyBotCharacter_3>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyBotCharacter_3::StaticClass()));
	if (BossCharacter)
	{
		AAIController* BossAIController = Cast<AAIController>(BossCharacter->GetController());
		if (BossAIController)
		{
			UBlackboardComponent* BlackboardComponent = BossAIController->GetBlackboardComponent();
			if (BlackboardComponent)
			{
				// Set the "BossStart" key to true
				//BlackboardComponent->SetValueAsBool(FName("BossStart"), true);
				BlackboardComponent->SetValueAsBool(FName("BossCinematicStart"), true);
			}
		}
	}
	// 16초 뒤에 Boss UI를 표시하기 위한 타이머 설정
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossTriggetActor::DisplayBossUI, 16.0f, false);
}



void ABossTriggetActor::DisplayBossUI()
{
	MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MainHUD && MainHUD->BossWidget)
	{
		MainHUD->DisPlayBossUI();
	}
}



void ABossTriggetActor::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 컨트롤러를 통해 MainHUD 가져오기
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
	}
}
