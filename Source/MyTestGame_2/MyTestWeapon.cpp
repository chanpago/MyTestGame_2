// Fill out your copyright notice in the Description page of Project Settings.

//game

#include "MyTestWeapon.h"
#include "MyTestCharacter_2.h"
#include "MyBasicCharacter_2.h"
#include "MyBotCharacter_3.h"
#include "MyBotCharacter_2.h"
#include "AIController_Magician.h"
#include "blackboard_keys.h"
#include "PlayerCharacter.h"

//engine
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

// Sets default values
AMyTestWeapon::AMyTestWeapon(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->CastShadow = true;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetBoxExtent(FVector(5.f, 5.f, 5.f));
	WeaponCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "DamageSocket");

	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	//static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	HitFX = ParticleAsset.Object;
}

void AMyTestWeapon::SetOwningPawn(AMyBasicCharacter_2* NewOwner)
{

	if (MyPawn != NewOwner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetOwningPawn");
		MyPawn = NewOwner;
	}
}


void AMyTestWeapon::IsNotPlayer(AMyBotCharacter_2* MyOwner)
{
	if (BotPawn != MyOwner)
		{
			
			BotPawn = MyOwner;
		}
}



void AMyTestWeapon::AttachMeshToPawn()
{

	if (MyPawn)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AttachMeshToPawn");
		USkeletalMeshComponent* PawnMesh = MyPawn->GetSpecificPawnMesh();
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
	else if (PlayerCharacter)
	{
		if (PlayerCharacter->CurrentWeapon)
		{
			USkeletalMeshComponent* PawnMesh = PlayerCharacter->GetSpecificPawnMesh();
			FName AttachPoint = PlayerCharacter->GetWeaponSecondAttachPoint();
			WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		}
		else
		{
			USkeletalMeshComponent* PawnMesh = PlayerCharacter->GetSpecificPawnMesh();
			FName AttachPoint = PlayerCharacter->GetWeaponAttachPoint();
			WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		}
		
	}
}


//ÀåÂø 
void AMyTestWeapon::OnEquip(const AMyTestWeapon* LastWeapon)
{
	AttachMeshToPawn();
}




void AMyTestWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (PlayerCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Attack Anemy");
		if (OtherActor->IsA(AMyBasicCharacter_2::StaticClass()) && OtherActor != PlayerCharacter && PlayerCharacter->DamageApply)
		{
			if (PlayerCharacter->AOWNUM)
			{
				UGameplayStatics::ApplyDamage(OtherActor,  50, PlayerCharacter->GetController(), this, UDamageType::StaticClass());
				PlayerCharacter->DamageApply = false;
			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, (PlayerCharacter->MyDamage) * PlayerCharacter->GetComboAttackNum(), PlayerCharacter->GetController(), this, UDamageType::StaticClass());
			}
		}
	}
	else if (MyPawn && (MyPawn->IsA(AMyBotCharacter_2::StaticClass()) || MyPawn->IsA(AMyBotCharacter_3::StaticClass())))
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* Emeny = Cast<APlayerCharacter>(OtherActor);
			if (Emeny && Emeny->GetCannotDamaged())
			{
				return;
			}

			AMyBotCharacter_3* Boss = Cast<AMyBotCharacter_3>(MyPawn);
			if (Boss && Boss->GetCanDamaged())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Anemy Attack Player");
				UGameplayStatics::ApplyDamage(OtherActor, 10.f, MyPawn->GetController(), this, UDamageType::StaticClass());
			}
		}
		else if (OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherActor != MyPawn && MyPawn->IsDuringAttack)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.f, MyPawn->GetController(), this, UDamageType::StaticClass());
			MyPawn->IsDuringAttack = false;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ERROR");
	}
	
}
