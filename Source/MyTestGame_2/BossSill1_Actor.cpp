// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSill1_Actor.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossSill1_Actor::ABossSill1_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossSkillMesh"));
    StaticMeshComponent->CastShadow = true;
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SlashPrimary");
	if (CollisionComponent)
	{
		CollisionComponent->SetBoxExtent(FVector(200.0f, 200.0f, 800.0f));
		CollisionComponent->SetCollisionProfileName("Bot_Projectile");
		RootComponent = CollisionComponent;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "CollisionComp");
	}
}

// Called when the game starts or when spawned
void ABossSill1_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossSill1_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossSill1_Actor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10, NULL, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "ABossSill1_Actor NotifyActorBeginOverlap");
		Destroy();
	}
}

