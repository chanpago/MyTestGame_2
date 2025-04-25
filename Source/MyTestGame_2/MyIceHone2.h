// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyIceHone2.generated.h"

UCLASS()
class MYTESTGAME_2_API AMyIceHone2 : public AActor
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	AMyIceHone2();

	FORCEINLINE class UBoxComponent* GetProjectileCollision()const { return CollisionComp; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	virtual void PostInitializeComponents()override;

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	class UBoxComponent* CollisionComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
