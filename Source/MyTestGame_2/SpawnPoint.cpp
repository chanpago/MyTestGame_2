// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "SpawnPoint.h"
#include "MyBasicCharacter_2.h"
#include "MyTestCharacter_2.h"

//engine
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bSpawn = false;
	//currentTime = 4.0f;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnPointCom"));
	CollisionSphere->InitSphereRadius(1000.0f);
	RootComponent = CollisionSphere;

}



// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 1000.0f, 50, FColor::Green, true, -1, 0, 2);

}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawn)
	{
		// Spawn bots at the location of the spawn point
		FVector Location = GetActorLocation();
		FRotator Rotation = FRotator::ZeroRotator;

		for (int i = 0; i < MyBots.Num(); ++i)
		{
			AMyBotCharacter_2* const myNewBot = GetWorld()->SpawnActor<AMyBotCharacter_2>(MyBots[i], Location, Rotation);
		}

		// Reset bSpawn
		bSpawn = false;
	}

}

void ASpawnPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpawnPoint::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ASpawnPoint::OnOverlapEnd);

}

void ASpawnPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMyTestCharacter_2::StaticClass()))
	{
		if (!bSpawn)
		{
			bSpawn = true;
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, " Player In!");
		}
	}
}

void ASpawnPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<APawn>())  // Assuming the player character is a Pawn
	{
		// Set bSpawn to false when the player character leaves the overlap area
		bSpawn = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, " Player Out!");
	}
}


