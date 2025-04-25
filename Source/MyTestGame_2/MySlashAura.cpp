// Fill out your copyright notice in the Description page of Project Settings.
//game
#include "MySlashAura.h"
#include "MyBasicCharacter_2.h"
#include "MyBotCharacter_2.h"
#include "MyBossCharacter.h"
#include "PlayerCharacter.h"
#include "MyTestWeapon.h"
#include "MyIceHone.h"
#include "MyIceHone2.h"
#include "MyBotCharacter_3.h"

//engine
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMySlashAura::AMySlashAura()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SlashPrimary");
	if (CollisionComp)
	{
		CollisionComp->SetBoxExtent(FVector(100.0f, 40.0f, 190.0f));
		CollisionComp->SetCollisionProfileName("Bot_Projectile");
		RootComponent = CollisionComp;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "CollisionComp");
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	
		ProjectileMovement->UpdatedComponent = CollisionComp;
		ProjectileMovement->InitialSpeed = 2000.0f;
		ProjectileMovement->MaxSpeed = 2000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ProjectileMovement");
	

	InitialLifeSpan = 3.0f;
}

void AMySlashAura::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMySlashAura::OnOverlapBegin);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnOverlapBegin");
	}
}

void AMySlashAura::SetOwningSlash(AMyBasicCharacter_2* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetOwningPawn");
		MyPawn = NewOwner;
	}
}

void AMySlashAura::SetOwningSlash(APlayerCharacter* NewOwner)
{
	if (Player != NewOwner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetOwningPawn");
		Player = NewOwner;
	}
}

void AMySlashAura::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(AMyBotCharacter_2::StaticClass())|| OtherActor->IsA(AMyBossCharacter::StaticClass()) || OtherActor->IsA(AMyBotCharacter_3::StaticClass()))
	{
		if (!Player)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,"Warning!! no MyPawn");
		}
		else 
		{
			UGameplayStatics::ApplyDamage(OtherActor, 50.f * Player->MyLevel * 0.5, Player->GetController(), this, UDamageType::StaticClass());
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DAMAGE: %f"), 500.f * MyPawn->MyLevel * 0.5));
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, __FUNCTION__);				Destroy();
		}			
	}
	else if(OtherActor->IsA(AMyTestWeapon::StaticClass())) 
	{			
	}
	else 
	{	
		//Destroy();
	}
}

void AMySlashAura::BeginPlay()
{
	Super::BeginPlay();
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMySlashAura::OnOverlapBegin);
}

void AMySlashAura::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	//Super::NotifyActorBeginOverlap(OtherActor);
		if (OtherActor->IsA(AMyBotCharacter_2::StaticClass()))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Destroy Before");
			UGameplayStatics::ApplyDamage(OtherActor, 50.f * MyPawn->MyLevel, MyPawn->GetController(), this, UDamageType::StaticClass());
			Destroy();
		}
		else if (OtherActor->IsA(AMyTestWeapon::StaticClass())) {		
		}
		else {
			//Destroy();
		}
}

void AMySlashAura::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NomalImpulse, const FHitResult& Hit)
{
}

