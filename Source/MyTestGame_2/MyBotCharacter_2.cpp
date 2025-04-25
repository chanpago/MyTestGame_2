// Fill out your copyright notice in the Description page of Project Settings.
//game

#include "MyBotCharacter_2.h"
#include "MyBasicCharacter_2.h"
#include "MyTestWeapon.h"
#include "PlayerCharacter.h"

//engine
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyBotCharacter_2::AMyBotCharacter_2() 
{
	// 캐릭터 이동 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 50.0f; // 기본 걷기 속도를 원하는 값으로 설정

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMyBotCharacter_2::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//무기 구현 함수
	SpawnDefaultInventory();
}



//bt에서 특정 조건 부합시 공격
int AMyBotCharacter_2::MeleeAttack_Implementation()
{
	if (Montage)
	{
		
		PlayAnimMontage(Montage);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Montage!");
		IsDuringAttack = true;
	}	

	FTimerHandle TH_Attack_End;
	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &AMyBotCharacter_2::MeleeAttackEnd, 1.7f, false);
	
	return 0;
}

void AMyBotCharacter_2::MeleeAttackEnd()
{
	IsDuringAttack = false;
}


UAnimMontage* AMyBotCharacter_2::GetMontage() const
{
	return Montage;
}


void AMyBotCharacter_2::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	float BeHitAnumDuration = PlayAnimMontage(BeHitAnimMontage);
	if (BeHitAnimMontage) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "BeHitAnimMontage!!");
	}
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		IsDuringAttack = false;
	}
}

float AMyBotCharacter_2::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if(myGetDamage > 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
		this->ShowDamageText(Damage); // 데미지 텍스트 표시
		MyHealth -= myGetDamage;
		PlayAnimMontage(BeHitAnimMontage);
	}

	if (MyHealth <= 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
		Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
	}	
	return myGetDamage;
}




void AMyBotCharacter_2::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die!");
	MyHealth = FMath::Min(0.f, MyHealth);

	if (Inventory.Num() > 0)
	{
		for (AMyTestWeapon* Weapon : Inventory)
		{
			if (Weapon)
			{
				Weapon->Destroy();
			}
		}

		Inventory.Empty(); // 무기 목록 초기화
	}
	if (!Killer) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "first Killer not on!");
	}

	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();
	///UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
		//UDamageType const* const DamageType = DamageEvent.DamageType ? Cast<UDamageType const>(DamageEvent.DamageType->GetDefaultObject()) : GetDefault<UDamageType const>();


	Killer = GetDamageInstigator(Killer, *DamageType);

	if (Killer) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Killer on!");
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Killer->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->AddEXP(10);
		}
	}
	if (!Killer) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Killer not on!");
	}


	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnimMontage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Duration: %f"), DeathAnimDuration));
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBasicCharacter_2::DeathAnimationEnd, DeathAnimDuration, false);
	
}