// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "BossAttach2.h"
#include "MyBossCharacter.h"

//engine
#include "Engine.h"

// Sets default values
ABossAttach2::ABossAttach2(const class FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SocketMesh2 = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SocketMesh"));
	SocketMesh2->CastShadow = true;
	SocketMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = SocketMesh2;

}

void ABossAttach2::AttachMeshToBoss()
{
	if (MyBoss)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AttachMeshToPawn");
		USkeletalMeshComponent* PawnMesh = MyBoss->GetSpecificPawnMesh2();
		FName AttachPoint = MyBoss->GetMeshAttachPoint2();
		SocketMesh2->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

void ABossAttach2::OnEquipMesh(const ABossAttach2* LastMesh)
{
	AttachMeshToBoss();
}

void ABossAttach2::SetOwningMesh(AMyBossCharacter* NewOwner)
{
	if (MyBoss != NewOwner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetOwningPawn");
		MyBoss = NewOwner;
	}
}

