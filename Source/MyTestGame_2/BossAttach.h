// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BossAttach.generated.h"


class AMyBossCharacter;

UCLASS()
class MYTESTGAME_2_API ABossAttach : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossAttach(const class FObjectInitializer& ObjectInitializer);

	void AttachMeshToBoss();

	void OnEquipMesh(const ABossAttach* LastMesh);

	void SetOwningMesh(AMyBossCharacter* NewOwner);

	UPROPERTY(VisibleDefaultsOnly, Category = Socket)
	USkeletalMeshComponent* SocketMesh;
	
protected:
	class AMyBossCharacter* MyBoss;



};
