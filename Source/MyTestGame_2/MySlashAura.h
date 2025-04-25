// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySlashAura.generated.h"

class APlayerCharacter;
class AMyBasicCharacter_2;

UCLASS()
class MYTESTGAME_2_API AMySlashAura : public AActor
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	AMySlashAura();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NomalImpulse, const FHitResult& Hit);

	FORCEINLINE class UBoxComponent* GetProjectileCollision()const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement()const { return ProjectileMovement; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents()override;

	void SetOwningSlash(AMyBasicCharacter_2* NewOwner);

	void SetOwningSlash(APlayerCharacter* NewOwner);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditDefaultsOnly, Category = State)
	float myInitialSpeed;

	UPROPERTY(EditDefaultsOnly, Category = State)
	float myMaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = State)
	FName ProjectileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	class AMyBasicCharacter_2* MyPawn;
	class AMyTestWeapon* Weapon;
	class APlayerCharacter* Player;

};
