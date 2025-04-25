//game
#include "MiddleSkill1IceActor.h"
#include "MyBasicCharacter_2.h"
#include "MyTestCharacter_2.h"
#include "PlayerCharacter.h"

//engine
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMiddleSkill1IceActor::AMiddleSkill1IceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// CollisionComp를 SphereComponent로 변경
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	if (CollisionComp)
	{
		// Sphere의 반지름 설정
		CollisionComp->InitSphereRadius(30.0f);
		CollisionComp->SetCollisionProfileName("Bot_Projectile");
		RootComponent = CollisionComp;
	}

	// ProjectileMovementComponent 초기화
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	if (ProjectileMovement)
	{
		ProjectileMovement->UpdatedComponent = CollisionComp;
		ProjectileMovement->InitialSpeed = 3000.f;
		ProjectileMovement->MaxSpeed = 3000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = false;
	}

	// Niagara Component 생성
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	if (NiagaraComp)
	{
		NiagaraComp->SetupAttachment(RootComponent);
		NiagaraComp->bAutoActivate = false;
	}

	InitialLifeSpan = 4.5f;
}

void AMiddleSkill1IceActor::BeginPlay()
{
	Super::BeginPlay();

	// Niagara Effect 시작
	if (NiagaraComp && NiagaraEffect)
	{
		NiagaraComp->SetAsset(NiagaraEffect);
		NiagaraComp->Activate(true);
	}
}

void AMiddleSkill1IceActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Onoverlapbegin");
		UGameplayStatics::ApplyDamage(OtherActor, 10, NULL, this, UDamageType::StaticClass());
		Destroy();
	}
}

void AMiddleSkill1IceActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMiddleSkill1IceActor::OnOverlapBegin);
	}
}

void AMiddleSkill1IceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiddleSkill1IceActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10, NULL, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "NotifyActorBeginOverlap");
		Destroy();
	}
}