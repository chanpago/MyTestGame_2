// Fill out your copyright notice in the Description page of Project Settings.


//game
#include "PlayerCharacter.h"
#include "BossSkill2_DA.h"

//engine
#include "Components/BoxComponent.h"
#include "BossSill2_Actor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
ABossSill2_Actor::ABossSill2_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossSkillMesh"));
	StaticMeshComponent->CastShadow = true;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaticMeshComponent->SetGenerateOverlapEvents(true);

	

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComponent->SetupAttachment(StaticMeshComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 쿼리만 가능, 물리적으로는 상호작용하지 않음
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap); // 모든 채널에 대해 오버랩 응답
	CollisionComponent->SetGenerateOverlapEvents(true); // 오버랩 이벤트 생성 활성화
}

// Called when the game starts or when spawned
void ABossSill2_Actor::BeginPlay()
{
	Super::BeginPlay();
	
	// 7초 후에 엑터 삭제 타이머 설정
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossSill2_Actor::DestroyActor, 7.0f, false);
}

// Called every frame
void ABossSill2_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 현재 위치 가져오기
	FVector CurrentLocation = GetActorLocation();

	// 중력처럼 작용할 속도 설정 (예: 매 초 980 유닛)
	float GravitySpeed = 1200.0f;

	// Z축 방향으로 위치 이동
	FVector NewLocation = CurrentLocation;
	NewLocation.Z -= GravitySpeed * DeltaTime; // 시간에 따른 이동

	// 새로운 위치 설정
	SetActorLocation(NewLocation);
}

void ABossSill2_Actor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.f, NULL, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ABossSill2_Actor ApplyDamage!");

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagaraSystem, GetActorLocation());

		DestroyActor();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "NotifyActorBeginOverlap Ice Actor Spawn");
}

// 7초 후 엑터를 삭제하는 함수
void ABossSill2_Actor::DestroyActor()
{
	Destroy();
}