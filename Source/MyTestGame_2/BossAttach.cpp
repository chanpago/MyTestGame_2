// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "BossAttach.h"
#include "MyBossCharacter.h"

//engine
#include "Engine.h"

// Sets default values
ABossAttach::ABossAttach(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SocketMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SocketMesh"));
	SocketMesh->CastShadow = true;
	SocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = SocketMesh;
}

void ABossAttach::AttachMeshToBoss()
{
	if (MyBoss)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "AttachMeshToPawn");
		USkeletalMeshComponent* PawnMesh = MyBoss->GetSpecificPawnMesh();
		FName AttachPoint = MyBoss->GetMeshAttachPoint();
		SocketMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

void ABossAttach::OnEquipMesh(const ABossAttach* LastMesh)
{
	AttachMeshToBoss();
}

void ABossAttach::SetOwningMesh(AMyBossCharacter* NewOwner)
{
	if (MyBoss != NewOwner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetOwningPawn");
		MyBoss = NewOwner;
	}
}

