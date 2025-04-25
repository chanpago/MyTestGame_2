// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BossAttach2.generated.h"

class AMyBossCharacter;

UCLASS()
class MYTESTGAME_2_API ABossAttach2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossAttach2(const class FObjectInitializer& ObjectInitializer);

	void AttachMeshToBoss();

	void OnEquipMesh(const ABossAttach2* LastMesh);

	void SetOwningMesh(AMyBossCharacter* NewOwner);

	UPROPERTY(VisibleDefaultsOnly, Category = Socket2)
	USkeletalMeshComponent* SocketMesh2;

protected:
	class AMyBossCharacter* MyBoss;



};
