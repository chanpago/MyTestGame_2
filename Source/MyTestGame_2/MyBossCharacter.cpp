// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "MyBossCharacter.h"
#include "BossAttach.h"
#include "BossAttach2.h"
#include "PlayerHitDamage.h"
#include "MyTestCharacter_2.h"
#include "BossHPWidget.h"
#include "PlayerCharacter.h"
#include "MyAIBossController.h"

//engine
#include "SkeletalMeshMerge.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/SkeletalMesh.h"
#include "Components/WidgetComponent.h"
#include "Animation/Skeleton.h"
#include "NiagaraComponent.h"




// Sets default values
AMyBossCharacter::AMyBossCharacter() : BossHealth(BossMaxHealth)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UPlayerHitDamage> DamageTextWidgetFinder(TEXT("/Game/_My/UI/PHD_BP.PHD_BP_C"));
	DamageTextWidgetClass = DamageTextWidgetFinder.Class;

	
	//보스 HP 바 
	BossHPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BossHPValue"));
	
	if (BossHPWidgetComponent)
	{



		//BossHPWidgetComponent->SetupAttachment(RootComponent);
		//BossHPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		static ConstructorHelpers::FClassFinder<UUserWidget>BossHPWidgetClass(TEXT("/Game/_My/UI/BossHP.BossHP_C"));
		if (BossHPWidgetClass.Succeeded())
		{		
			BossHPWidgetComponent->SetWidgetClass(BossHPWidgetClass.Class);
		}
	}
	
}

USkeletalMeshComponent* AMyBossCharacter::GetSpecificPawnMesh() const
{
	return GetMesh();
}

FName AMyBossCharacter::GetMeshAttachPoint() const
{
	return MeshAttachPoint;
}

USkeletalMeshComponent* AMyBossCharacter::GetSpecificPawnMesh2() const
{
	return GetMesh();
}

FName AMyBossCharacter::GetMeshAttachPoint2() const
{
	return MeshAttachPoint2;
}





void AMyBossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultMesh();
	SpawnDefaultMesh2();
}

float AMyBossCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Boss Take Damage On!");

	if (BossHealth <= 0.0f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boss HP: %f"), BossHealth));
		return 0.0f;
	}
	if (myGetDamage > 0.f)
	{
		BossHealth -= myGetDamage;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boss HP: %f"), BossHealth));
	}
	if (BossHealth <= BossMaxHealth / 2) {
		if (SocketSkeletalMesh.Num() > 0)
		{
			for (ABossAttach* MeshSkel : SocketSkeletalMesh)
			{
				if (MeshSkel)
				{
					MeshSkel->Destroy();
				}
			}
			SocketSkeletalMesh.Empty(); // 매쉬 목록 초기화
		}
	}
	if (BossHealth <= 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
		Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(myGetDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "On Hit!!");
	}

	return myGetDamage;
}

void AMyBossCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	float BeHitAnimDuration = PlayAnimMontage(BeHitAnimMontage);
	if (BeHitAnimMontage) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "BeHitAnimMontage!!");
	}
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);		
	}
}

void AMyBossCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	BossHealth = FMath::Min(0.f, BossHealth);

	//보스사망시 붙어있는 소캣 삭제	
		
	
	if (SocketSkeletalMesh2.Num() > 0)
	{
		for (ABossAttach2* MeshSkel : SocketSkeletalMesh2)
		{
			if (MeshSkel)
			{
				MeshSkel->Destroy();
			}
		}
		SocketSkeletalMesh2.Empty(); // 매쉬 목록 초기화
	}



	

	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();
	///UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
		//UDamageType const* const DamageType = DamageEvent.DamageType ? Cast<UDamageType const>(DamageEvent.DamageType->GetDefaultObject()) : GetDefault<UDamageType const>();


	Killer = GetDamageInstigator(Killer, *DamageType);

	if (Killer) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Killer on!");
		AMyTestCharacter_2* PlayerCharacter = Cast<AMyTestCharacter_2>(Killer->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->AddEXP(100);
		}
	}
	if (!Killer) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Killer not on!");
	}



	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnimMontage);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Duration: %f"), DeathAnimDuration));
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossCharacter::DeathAnimationEnd, DeathAnimDuration - 0.3f, false);
	//PlayAnimMontage(DeathAnimMontage,4.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 2!");
	/*
	AMyBotCharacter_2* BotCharacter = Cast<AMyBotCharacter_2>(this);
	if (BotCharacter && Killer)
	{
		UE_LOG(LogTemp, Warning, TEXT("BotCharacter && Killer loaded Successfully"));
		AMyTestCharacter_2* PlayerCharacter = Cast<AMyTestCharacter_2>(Killer->GetPawn());
		if (PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter loaded Successfully"));
			// 봇 캐릭터에 경험치 부여
			PlayerCharacter->MyEXP += 10;
		}
	}
	*/

}

void AMyBossCharacter::DeathAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

void AMyBossCharacter::ShowDamageText(float DamageAmount)
{
	//UE_LOG(LogTemp, Warning, TEXT("ShowDamageText loaded Successfully"));

	if (DamageTextWidgetClass)
	{
		auto const HitDamageWidget = CreateWidget<UPlayerHitDamage>(GetWorld(), DamageTextWidgetClass);

		if (HitDamageWidget)
		{
			HitDamageWidget->AddToViewport();

			//UE_LOG(LogTemp, Warning, TEXT("SetHitDamage loaded Successfully"));

			// 데미지 양을 문자열로 변환하여 텍스트에 표시
			HitDamageWidget->SetHitDamage(DamageAmount);


			/*
			FTimerHandle TimerHandle;
			float MoveTime = 0.5f; // 이동하는 시간 (초)
			float MoveDistance = 50.0f; // 이동 거리 (Z축 방향)

			GetWorldTimerManager().SetTimer(TimerHandle, [HitDamageWidget, MoveDistance]() {
				//화면에서 삭제하는 코드
				HitDamageWidget->RemoveFromParent();
				}, 0.5f, false);
			*/

		}
	}
}

void AMyBossCharacter::AnimNotify_SpawnNiagaraSystem(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (NiagaraSystemTemplate)
	{
		FVector Location = MeshComp->GetComponentLocation();
		FRotator Rotation = MeshComp->GetComponentRotation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystemTemplate, Location, Rotation);
	}
}

void AMyBossCharacter::SetCurrentMesh(ABossAttach* NewMesh, ABossAttach* LastMesh)
{
	ABossAttach* LocalLastWeapon = NULL;
	if (LastMesh != NULL)
	{
		LocalLastWeapon = LastMesh;
	}

	if (NewMesh)
	{
		NewMesh->SetOwningMesh(this);
		NewMesh->OnEquipMesh(LastMesh);
	}
}

void AMyBossCharacter::SetCurrentMesh(ABossAttach2* NewMesh, ABossAttach2* LastMesh)
{
	ABossAttach2* LocalLastWeapon2 = NULL;
	if (LastMesh != NULL)
	{
		LocalLastWeapon2 = LastMesh;
	}

	if (NewMesh)
	{
		NewMesh->SetOwningMesh(this);
		NewMesh->OnEquipMesh(LastMesh);
	}
}

void AMyBossCharacter::EquipSkelMesh(ABossAttach* SkelMesh)
{
	if (SkelMesh)
	{
		SetCurrentMesh(SkelMesh, CurrentMesh);
	}
}

void AMyBossCharacter::EquipSkelMesh(ABossAttach2* SkelMesh)
{
	if (SkelMesh)
	{
		SetCurrentMesh(SkelMesh, CurrentMesh2);
	}
}

UAnimMontage* AMyBossCharacter::GetMontage() const
{
	return Montage;
}

int AMyBossCharacter::Skill_Implementation()
{

	 if (Montage)
    {
        // 몽타주를 재생하고, 재생 여부를 반환하는 함수를 호출합니다.
        float PlayRate = 1.0f; // 재생 속도를 설정합니다.
        FOnMontageEnded EndDelegate; // 몽타주 재생이 끝나면 호출될 델리게이트입니다.
        int32 length = PlayAnimMontage(Montage, PlayRate);
        
        // 몽타주가 성공적으로 재생되었는지 확인합니다.
        if (length != 0)
        {
            // 몽타주가 성공적으로 재생됨
            //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Montage successfully played");
        }
        else
        {
            // 몽타주 재생이 실패함
            //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Failed to play montage");
        }
    }
    else
    {
        // 몽타주를 찾을 수 없음
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No montage found");
    }

	

	return 0;
}



void AMyBossCharacter::SkillEnd()
{

}


void AMyBossCharacter::SpawnDefaultMesh()
{
	int32 NumMeshClasses = DefaultMeshClass.Num();

	for (int32 i = 0; i < NumMeshClasses; i++)
	{
		if (DefaultMeshClass[0])
		{
			FActorSpawnParameters SpawnInfo;
			UWorld* WRLD = GetWorld();
			ABossAttach* NewMesh = WRLD->SpawnActor<ABossAttach>(DefaultMeshClass[0], SpawnInfo);
			if (NewMesh) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NewMesh on!");
			}
			AddMesh(NewMesh);
		}
		if (SocketSkeletalMesh.Num() > 0)
		{
			EquipSkelMesh(SocketSkeletalMesh[0]);
		}
	}
}

void AMyBossCharacter::SpawnDefaultMesh2()
{
	int32 NumMeshClasses = DefaultMeshClass2.Num();

	for (int32 i = 0; i < NumMeshClasses; i++)
	{
		if (DefaultMeshClass2[0])
		{
			FActorSpawnParameters SpawnInfo;
			UWorld* WRLD2 = GetWorld();
			ABossAttach2* NewMesh = WRLD2->SpawnActor<ABossAttach2>(DefaultMeshClass2[0], SpawnInfo);
			if (NewMesh) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NewMesh on!");
			}
			AddMesh(NewMesh);
		}
		if (SocketSkeletalMesh2.Num() > 0)
		{
			EquipSkelMesh(SocketSkeletalMesh2[0]);
		}
	}
}

void AMyBossCharacter::AddMesh(ABossAttach* MeshSkel)
{
	if (MeshSkel)
	{
		SocketSkeletalMesh.AddUnique(MeshSkel);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NewMesh2 on!");
	}
}

void AMyBossCharacter::AddMesh(ABossAttach2* MeshSkel)
{
	if (MeshSkel)
	{
		SocketSkeletalMesh2.AddUnique(MeshSkel);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NewMesh head  on!");
	}
}

// Called when the game starts or when spawned
void AMyBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	BossHealth = BossMaxHealth;

	// 나이아가라 시스템 설정
	if (NiagaraSystemTemplate)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), NiagaraSystemTemplate, GetActorLocation(), GetActorRotation()
		);
		if (NiagaraComponent)
		{
			//NiagaraComponent->OnSystemFinished.AddDynamic(this, &AMyBossCharacter::HandleNiagaraCollision);
		}
	}

}

/*
void AMyBossCharacter::OnNiagaraParticleCollision(FVector ParticlePosition, FVector ParticleVelocity, FVector ParticleSize, const FName& Tag, AActor* Instigator)
{

}
*/

void AMyBossCharacter::OnNiagaraCollisionEvent(FVector ParticlePosition, FVector ParticleVelocity, FVector ParticleSize, FName EventName)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->MyHealth -= 10.0f;
		if (PlayerCharacter->MyHealth <= 0.0f)
		{
			// 플레이어 캐릭터가 사망한 경우 처리 로직을 추가합니다.
		}
	}
}

void AMyBossCharacter::HandleNiagaraCollision(FVector ParticlePosition, FVector ParticleVelocity, FVector ParticleSize, FName EventName)
{
	OnNiagaraCollisionEvent(ParticlePosition, ParticleVelocity, ParticleSize, EventName);
}



// Called every frame
void AMyBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	auto const HudHp = Cast<UBossHPWidget>(BossHPWidgetComponent->GetUserWidgetObject());
	if (HudHp)
	{
		HudHp->AddToViewport();
		HudHp->SetHPBarValuePercent((BossHealth / BossMaxHealth * 100) * 0.01f);
		
	}
	
}

// Called to bind functionality to input
void AMyBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




/*
float AMyBossCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	const float BossGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (BossHealth <= 0.0f) {
		return 0.0f;
	}

	if (BossGetDamage > 0.f)
	{
		BossHealth -= BossGetDamage;
	}

	
}
*/

