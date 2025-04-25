 // Fill out your copyright notice in the Description page of Project Settings.

//game
#include "MyBasicCharacter_2.h"
#include "MyTestWeapon.h"
#include "HPbar.h"
#include "HpExpBar.h"
#include "HUD_InGame.h"
#include "AIController.h"
#include "MySlashAura.h"
#include "PlayerCharacter.h"
#include "PlayerHitDamage.h"

//engine
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Containers/UnrealString.h"
#include "MyTestGame_2GameModeBase.h"

// Sets default values
AMyBasicCharacter_2::AMyBasicCharacter_2() : MyHealth(MyMaxHealth), WidgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	MyHPbarText = FString::SanitizeFloat(MyHealth) + "/" + FString::SanitizeFloat(MyMaxHealth);
	/*
	if (WidgetComponent)
	{
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
		//캐릭터 머리위에 붙히는 
		static ConstructorHelpers::FClassFinder<UUserWidget>WidgetClass(TEXT("/Game/_My/UI/HPBAR.HPBAR_C"));

		if (WidgetClass.Succeeded())
		{
			//UE_LOG(LogTemp, Warning, TEXT("WC loaded Successfully"));
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, __FUNCTION__);
			WidgetComponent->SetWidgetClass(WidgetClass.Class);

		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("WC not loaded Successfully"));
		}
	}
	
	static ConstructorHelpers::FClassFinder<UPlayerHitDamage> DamageTextWidgetFinder(TEXT("/Game/_My/UI/PHD_BP.PHD_BP_C"));
	DamageTextWidgetClass = DamageTextWidgetFinder.Class;
	*/
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Charge.P_Charge'"));

	//static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	ChargeFX = ParticleAsset.Object;

	MySlashInstance = nullptr;
}

void AMyBasicCharacter_2::ShowDamageText(float DamageAmount)
{
	if (DamageTextWidgetClass)
	{
		UPlayerHitDamage* HitDamageWidget = CreateWidget<UPlayerHitDamage>(GetWorld(), DamageTextWidgetClass);

		if (HitDamageWidget)
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation(FName("DamageWidget"));

			UWidgetComponent* DamageWidgetComponent = NewObject<UWidgetComponent>(this);
			DamageWidgetComponent->SetWidget(HitDamageWidget);
			DamageWidgetComponent->SetWorldLocation(SocketLocation);
			DamageWidgetComponent->SetDrawSize(FVector2D(20, 20));
			DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			DamageWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			DamageWidgetComponent->RegisterComponent();

			HitDamageWidget->SetHitDamage(DamageAmount);

			// 애니메이션 재생
			HitDamageWidget->PlayDamageTextAnimation();

			// 위젯 제거와 이동 처리
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DamageWidgetComponent]()
				{
					if (DamageWidgetComponent)
					{
						DamageWidgetComponent->DestroyComponent();
					}
				}, 1.0f, false);

			FVector StartLocation = SocketLocation;
			FVector EndLocation = StartLocation + FVector(0, 0, 50);

			FTimerHandle AnimationTimerHandle;
			float Duration = 0.5f;

			// DeltaSeconds를 미리 계산
			float DeltaTime = GetWorld()->GetDeltaSeconds();

			// 타이머 설정
			GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, [DamageWidgetComponent, StartLocation, EndLocation, DeltaTime]()
				{
					if (DamageWidgetComponent)
					{
						FVector CurrentLocation = DamageWidgetComponent->GetComponentLocation();
						FVector NewLocation = FMath::VInterpTo(CurrentLocation, EndLocation, DeltaTime, 0.5f);

						DamageWidgetComponent->SetWorldLocation(NewLocation);
					}
				}, DeltaTime, false);
		}
	}
}

float AMyBasicCharacter_2::GetHealthValue()
{
	return MyHealth;
}


void AMyBasicCharacter_2::SetupStimulus()
{
}

// Called every frame
void AMyBasicCharacter_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//make HPbar on the character head above
	auto const uw = Cast<UHPbar>(WidgetComponent->GetUserWidgetObject());
	//auto const HUDHP = Cast<UHUD_InGame>(WidgetComponent->GetUserWidgetObject());

	MyHPnum = (MyHealth / MyMaxHealth * 100) * 0.01f;
	//UE_LOG(LogTemp, Log, TEXT("Character HP:"), myHPnum);

	MyHPbarText = FString::SanitizeFloat(MyHealth) + "/" + FString::SanitizeFloat(MyMaxHealth);

	//UE_LOG(LogTemp, Warning, TEXT("UW loaded Successfully"));
	/*
	uw->SetBarValuePercent(MyHPnum);
	if (uw)
	{
		uw->SetVisibility(ESlateVisibility::Collapsed); // 위젯 숨기기
	}
	*/
}

// Called when the game starts or when spawned
void AMyBasicCharacter_2::BeginPlay()
{
	Super::BeginPlay();

	MyHealth = MyMaxHealth;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, MyCharacterName.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT(" HP : %f"), MyHealth));
}

//weaponstart

USkeletalMeshComponent* AMyBasicCharacter_2::GetSpecificPawnMesh() const
{
	return GetMesh();
}

FName AMyBasicCharacter_2::GetWeaponAttachPoint() const
{
	//->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Attachwweapon!");
	return WeaponAttachPoint;
}

void AMyBasicCharacter_2::EquipWeapon(AMyTestWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void AMyBasicCharacter_2::AddWeapon(AMyTestWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon);
	}
}

//최근무기 셋
void AMyBasicCharacter_2::SetCurrentWeapon(AMyTestWeapon* NewWeapon, AMyTestWeapon* LastWeapon)
{
	AMyTestWeapon* LocalLastWeapon = NULL;
	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		//NewWeapon->SetOwningBoss(this);
		NewWeapon->OnEquip(LastWeapon);
	}
}

void AMyBasicCharacter_2::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClass.Num();

	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClass[0])
		{
			FActorSpawnParameters SpawnInfo;
			UWorld* WRLD = GetWorld();
			AMyTestWeapon* NewWeapon = WRLD->SpawnActor<AMyTestWeapon>(DefaultInventoryClass[0], SpawnInfo);
			AddWeapon(NewWeapon);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "FirstWeapon on!");
		}
	}

	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}
//weaponend

// Called to bind functionality to input
void AMyBasicCharacter_2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyBasicCharacter_2::DeathAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

// Called every frame


// Called to bind functionality to input


void AMyBasicCharacter_2::AttackMelee()
{

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Attack!");
	PlayAnimMontage(AttackMeleeAnim, 1.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Attack!");
	IsDuringAttack = true;

	FTimerHandle TH_Attack_End;
	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &AMyBasicCharacter_2::AttackMeleeEnd, 1.7f, false);
	
}

void AMyBasicCharacter_2::AttackMeleeEnd()
{
	IsDuringAttack = false;
}

void AMyBasicCharacter_2::ComboMelee1()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Combo1!");
	PlayAnimMontage(ComboMelee1Anim, 1.0f);
	IsDuringAttack = true;

	FTimerHandle TH_Combo_End;
	GetWorldTimerManager().SetTimer(TH_Combo_End, this, &AMyBasicCharacter_2::ComboMelee1End, 1.7f, false);
}

void AMyBasicCharacter_2::ComboMelee2()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Combo2!");
	PlayAnimMontage(ComboMelee2Anim, 1.0f);
	IsDuringAttack = true;

	FTimerHandle TH_Combo_End;
	GetWorldTimerManager().SetTimer(TH_Combo_End, this, &AMyBasicCharacter_2::ComboMelee2End, 1.7f, false);
}

void AMyBasicCharacter_2::ComboMelee1End()
{
	IsDuringAttack = false;
}

void AMyBasicCharacter_2::ComboMelee2End()
{
	IsDuringAttack = false;
}

void AMyBasicCharacter_2::Slash()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ChargeFX, GetActorLocation());
	
	// 애니메이션 몽타주를 재생하고 애니메이션이 끝나면 OnSlashMontageEnded 함수를 호출하도록 설정
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SlashAnim)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyBasicCharacter_2::SlashEnd);
		AnimInstance->Montage_Play(SlashAnim, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash1");
		AnimInstance->Montage_SetEndDelegate(EndDelegate,SlashAnim);		
	}

}

void AMyBasicCharacter_2::SlashEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		// 애니메이션 몽타주가 정상적으로 종료된 경우에만 실행
		if (MySlash)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash2");
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(90.0f, 0.0f, 90.0f));
			//UWorld* const World = GetWorld();
			MySlashInstance = GetWorld()->SpawnActor<AMySlashAura>(MySlash, GetActorLocation(), GetActorRotation());
			if (MySlashInstance != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash3");
				//World->SpawnActor<AMySlashAura>(MySlash, SpawnLocation, SpawnRotation);
				MySlashInstance->SetOwningSlash(this);
				
			}
			
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No Slash Actor");
		}
	}
}

void AMyBasicCharacter_2::Evade()
{
	PlayAnimMontage(EvadeAnim, 1.0f);
}


float AMyBasicCharacter_2::GetHealth() const
{
	return MyHealth;
}

float AMyBasicCharacter_2::GetMaxHealth() const
{
	return MyMaxHealth;
}

void AMyBasicCharacter_2::SetHealth(float const NewHealth)
{
	MyHealth = NewHealth;
}

void AMyBasicCharacter_2::SetLevel(float const Level)
{
	MyLevel = Level;
}

