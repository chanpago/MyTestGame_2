// Fill out your copyright notice in the Description page of Project Settings.
//game
#include "MyBotCharacter_3.h"
#include "MyBasicCharacter_2.h"
#include "MyTestWeapon.h"
#include "PlayerCharacter.h"
#include "blackboard_keys.h"
#include "BossAnimInstance.h"
#include "BossWidget.h"
#include "MainHUD.h" // AMainHUD ��� ���� ����

//engine
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "AIController.h"
#include "Engine.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"


AMyBotCharacter_3::AMyBotCharacter_3()
{
	// ĳ���� �̵� �ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = 50.0f; // �⺻ �ȱ� �ӵ��� ���ϴ� ������ ����

	GetCharacterMovement()->bOrientRotationToMovement = true;
	MyMaxHealth = 1000;
	MyHealth = 1000;
	Groggy = 0;
	
	// ���� ���� ������Ʈ �ʱ�ȭ
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(RootComponent);
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidget->SetVisibility(false); // �⺻������ ��Ȱ��ȭ

	// ���� �̸� �ʱ�ȭ
	BossName = FText::FromString(TEXT("ù ��° �ݽ� ��������"));


	//���� HP �� 
	BossHPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BossHPValue"));

	if (BossHPWidgetComponent)
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>BossHPWidgetClass(TEXT("'/Game/_My/UI/BossWidget.BossWidget'"));
		if (BossHPWidgetClass.Succeeded())
		{
			BossHPWidgetComponent->SetWidgetClass(BossHPWidgetClass.Class);
		}
	}
	
}

void AMyBotCharacter_3::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//���� ���� �Լ�
	SpawnDefaultInventory();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsA(UBossAnimInstance::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "BossAnimInstance is set correctly!");
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "BossAnimInstance is NOT set correctly!");
	}
	
}

void AMyBotCharacter_3::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	
	if (BeHitAnimMontage) {
		PlayAnimMontage(BeHitAnimMontage);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "BeHitAnimMontage!!");
	}
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		IsDuringAttack = false;
	}
}

void AMyBotCharacter_3::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die!");
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

		Inventory.Empty(); // ���� ��� �ʱ�ȭ
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Duration: %f"), DeathAnimDuration));
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBasicCharacter_2::DeathAnimationEnd, DeathAnimDuration, false);
	//PlayAnimMontage(DeathAnimMontage,4.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 2!");
	/*
	AMyBotCharacter_2* BotCharacter = Cast<AMyBotCharacter_2>(this);
	if (BotCharacter && Killer)
	{
		UE_LOG(LogTemp, Warning, TEXT("BotCharacter && Killer loaded Successfully"));
		AMyTestCharacter_2* PlayerCharacter = Cast<AMyTestCharacter_2>(Killer->GetPawn());
		if (PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter loaded Successfully"));
			// �� ĳ���Ϳ� ����ġ �ο�
			PlayerCharacter->MyEXP += 10;s
		}
	}
	*/
	ShowBossUIDisplay();
}

float AMyBotCharacter_3::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// ������ Ű ����
	AAIController* AIController = Cast<AAIController>(this->GetController());
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	bool bIsBlockReady = BlackboardComp->GetValueAsBool(bb_keys::IsBlockReady);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(EventInstigator->GetPawn());

	/*
	if (bIsBlockReady)
	{
		PlayAnimMontage(BlockAnimMontage);

		PlayerCharacter->IsDuringAttack = false;

		UAnimInstance* PlayerAnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
		PlayerAnimInstance->Montage_Stop(0.2f, PlayerCharacter->AttackMeleeAnim);
		this->ShowDamageText(0);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Block!");
		BlackboardComp->SetValueAsBool(bb_keys::IsBlockReady, false);
	}
	else
	{
		if (myGetDamage > 0.f)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
			this->ShowDamageText(Damage); // ������ �ؽ�Ʈ ǥ��
			MyHealth -= myGetDamage;
			PlayAnimMontage(BeHitAnimMontage);
		}
	}
	*/
	if (IsGroggy) {
		this->ShowDamageText(Damage);
		TakeDamageWhileGroggy();
		MyHealth -= myGetDamage;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Groggy Hit!");
		//PlayAnimMontage(GroggyHit);
	}

	if (myGetDamage >= 20.f && !IsGroggy)
	{

		this->ShowDamageText(Damage); // ������ �ؽ�Ʈ ǥ��
		MyHealth -= myGetDamage;
		PlayAnimMontage(BeHitAnimMontage);
		Groggy += 2;
	}

	if (myGetDamage > 0.f && !IsGroggy)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
		this->ShowDamageText(Damage); // ������ �ؽ�Ʈ ǥ��
		MyHealth -= myGetDamage;
		PlayAnimMontage(lightHitedAnimMontage);
		//PlayAnimMontage(BeHitAnimMontage);
		Groggy++;
	}

	if (MyHealth <= 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
		Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
	}

	return myGetDamage;
}

void AMyBotCharacter_3::SetLockOnWidgetVisibility(bool bIsVisible)
{
	
	if (LockOnWidget)
	{
		LockOnWidget->SetVisibility(bIsVisible);
	}
	
}

void AMyBotCharacter_3::SetCanDamaged(bool value){CanDamaged = value;}

bool AMyBotCharacter_3::GetCanDamaged(){return CanDamaged;}

void AMyBotCharacter_3::GroggyState()
{
	UBossAnimInstance* AnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	Groggy = 0;
	IsGroggy = true;

	AnimInstance->IsHitWhileGroggy = true;

	this->SetCanDamaged(false);

	// �����忡 �����Ͽ� Groggy ������ true�� ����
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController) {
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp) {
			BlackboardComp->SetValueAsBool(FName("Groggy"), true);
		}

		// ������ �׷α� ���¿� ���� �� �÷��̾� ��Ŀ���� ����
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	// �÷��̾� ĳ���� �ҷ�����
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//�׷α� ���� ��
	PlayerCharacter->GroggyAttack = true;
	
	PlayAnimMontage(GroggyStart);

	


	// 5�� �Ŀ� GroggyEndState ȣ��
	GetWorldTimerManager().SetTimer(GroggyTimerHandle, this, &AMyBotCharacter_3::GroggyEndState, 5.0f, false);

}



void AMyBotCharacter_3::GroggyEndState()
{
	IsGroggy = false;
	UBossAnimInstance* AnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->IsHitWhileGroggy = false;
	PlayAnimMontage(GroggyEnd);

	// �÷��̾� ĳ���� �ҷ�����
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerCharacter->GroggyAttack = false;
	// �����忡 �����Ͽ� Groggy ������ false�� ����
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController) {
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp) {
			BlackboardComp->SetValueAsBool(FName("Groggy"), false);
		}
	}

}

void AMyBotCharacter_3::TakeDamageWhileGroggy()
{
	UBossAnimInstance* AnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && GroggyStart && GroggyHit) {
		// Groggy ���¿��� Hit ���·� ��ȯ
		AnimInstance->IsHitWhileGroggy = true;

		// GroggyHit �ִϸ��̼��� ����ϸ鼭 ���� �ӽſ��� IsHitWhileGroggy�� ��ȯ
		AnimInstance->Montage_Play(GroggyHit);
	}
}

void AMyBotCharacter_3::ShowBossUIDisplay()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			MainHUD->BossDieDisplay(); // UI ǥ��

			// 3�� �� UI ����� Ÿ�̸� ����
			FTimerHandle HideUITimerHandle;
			GetWorldTimerManager().SetTimer(HideUITimerHandle, this, &AMyBotCharacter_3::HideBossUIDisplay, 3.0f, false);
		}
	}
}

void AMyBotCharacter_3::HideBossUIDisplay()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			MainHUD->HideBossDieDisplay(); // UI �����
		}
	}
}

void AMyBotCharacter_3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Groggy: %f"), Groggy));
	if (Groggy > 0) {

		Groggy -= 0.005;
	}
	
	if (Groggy >= 8) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		PlayerCharacter->GroggyAttackAttempt = true;
		GroggyState();
		
	}

	// �÷��̾� ĳ���� �ҷ�����
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		// ������ �÷��̾��� ����� ��ġ�� ���
		FVector RelativeLocation = GetTransform().InverseTransformPosition(PlayerCharacter->GetActorLocation());

		// �÷��̾ ������ ���ʿ� Ư�� �Ÿ� ���� ��ġ�� �ִ��� Ȯ��
		if (RelativeLocation.Y > 0 && FMath::Abs(RelativeLocation.Y) <= 10.0f && IsGroggy)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "GroggySpoton!");
			PlayerCharacter->GroggyState = true;

			// ������ �÷��̾� ���̿� ������ �׸��ϴ�.
			FVector BossLocation = GetActorLocation();
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			DrawDebugLine(GetWorld(), BossLocation, PlayerLocation, FColor::Red, false, -1, 0, 5.0f);
		}
		else {
			PlayerCharacter->GroggyState = false;
		}
	}

	/*
	if (BossWidget)
	{
		BossWidget->SetBossHPBarValuePercent((this->MyHealth / this->MyMaxHealth * 100) * 0.01f);
	}
	*/
}



//bt���� Ư�� ���� ���ս� ����
int AMyBotCharacter_3::MeleeAttack_Implementation()
{
	if (Montage)
	{

		PlayAnimMontage(Montage);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Montage!");
		IsDuringAttack = true;
	}

	FTimerHandle TH_Attack_End;
	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &AMyBotCharacter_3::MeleeAttackEnd, 1.7f, false);

	return 0;
}

void AMyBotCharacter_3::MeleeAttackEnd()
{
	IsDuringAttack = false;
}


UAnimMontage* AMyBotCharacter_3::GetMontage() const
{
	return Montage;
}
