//game
#include "MyIceHone.h"
#include "PlayerCharacter.h" 

//engine
#include "Components/BoxComponent.h" 
#include "Engine/Engine.h" 
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/DamageType.h" 


// 기본 값 설정
AMyIceHone::AMyIceHone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SlashPrimary");
	if (CollisionComp)
	{
		CollisionComp->SetBoxExtent(FVector(150.0f, 150.0f, 500.0f));
		CollisionComp->SetCollisionProfileName("Bot_Projectile");
		RootComponent = CollisionComp;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "CollisionComp");
	}

	InitialLifeSpan = 2.5f;
}

void AMyIceHone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{


	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Onoverlapbegin");
		UGameplayStatics::ApplyDamage(OtherActor, 10, NULL, this, UDamageType::StaticClass());

		Destroy();

	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Slash OnOverlapbegin");


}




// Called when the game starts or when spawned
void AMyIceHone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyIceHone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyIceHone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10, NULL, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "NotifyActorBeginOverlap");
		Destroy();
	}
}

void AMyIceHone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyIceHone::OnOverlapBegin);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnOverlapBegin");
	}
}

