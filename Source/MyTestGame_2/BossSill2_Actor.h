// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSill2_Actor.generated.h"


class UBoxComponent;
class UNiagaraSystem;

UCLASS()
class MYTESTGAME_2_API ABossSill2_Actor : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	FTimerHandle TimerHandle;

	// Niagara system to play on hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* HitNiagaraSystem;

	// Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Collision Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	ABossSill2_Actor();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyActor();
};
