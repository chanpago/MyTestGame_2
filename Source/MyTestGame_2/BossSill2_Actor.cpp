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
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // ������ ����, ���������δ� ��ȣ�ۿ����� ����
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap); // ��� ä�ο� ���� ������ ����
	CollisionComponent->SetGenerateOverlapEvents(true); // ������ �̺�Ʈ ���� Ȱ��ȭ
}

// Called when the game starts or when spawned
void ABossSill2_Actor::BeginPlay()
{
	Super::BeginPlay();
	
	// 7�� �Ŀ� ���� ���� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossSill2_Actor::DestroyActor, 7.0f, false);
}

// Called every frame
void ABossSill2_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ���� ��ġ ��������
	FVector CurrentLocation = GetActorLocation();

	// �߷�ó�� �ۿ��� �ӵ� ���� (��: �� �� 980 ����)
	float GravitySpeed = 1200.0f;

	// Z�� �������� ��ġ �̵�
	FVector NewLocation = CurrentLocation;
	NewLocation.Z -= GravitySpeed * DeltaTime; // �ð��� ���� �̵�

	// ���ο� ��ġ ����
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

// 7�� �� ���͸� �����ϴ� �Լ�
void ABossSill2_Actor::DestroyActor()
{
	Destroy();
}