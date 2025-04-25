// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBasicCharacter_2.h"
#include "MyBotCharacter_2.h"
#include "SpawnPoint.generated.h"

UCLASS()
class MYTESTGAME_2_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	ASpawnPoint();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, Category = "MyBot")
	TArray<TSubclassOf<class AMyBotCharacter_2>>MyBots;

	AMyBasicCharacter_2* MySpawn_Bot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float currentTime;

	bool bSpawn;

};
