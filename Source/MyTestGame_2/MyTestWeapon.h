// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyTestWeapon.generated.h"

//class APlayerCharacter* PlyaerCharacter;

class AMyBasicCharacter_2;
class AMyTestCharacter_2;
class APlayerCharacter;
class AMyBotCharacter_3;
class AMyBotCharacter_2;

UCLASS()
class MYTESTGAME_2_API AMyTestWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	AMyTestWeapon(const class FObjectInitializer& ObjectInitializer);

	//이 무기를 가질 엑터 선언
	FORCEINLINE void SetOwningPawnCharacter(APlayerCharacter* Character) { PlayerCharacter = Character; };

	FORCEINLINE APlayerCharacter* GetOwningPawnCharacter() { return PlayerCharacter; };

	//이 무기를 가질 엑터 선언
	void SetOwningPawn(AMyBasicCharacter_2* NewOwner);
	
	//메쉬를 폰에 붙히는 함수
	void AttachMeshToPawn();
	//어떤 무기를 붙힐지
	void OnEquip(const AMyTestWeapon* LastWeapon);

	void IsNotPlayer(AMyBotCharacter_2* Owner);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	APlayerCharacter* GetOwningOawn() { return PlayerCharacter; };

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditDefaultsOnly, Category = "MyFX")
	UParticleSystem* HitFX;

	bool BlockAvailable;
	bool MiddleBossCheck = true;

protected:
	class AMyBasicCharacter_2* MyPawn;
	class AMyTestCharacter_2* TestPawn;
	class AMyBotCharacter_2* BotPawn;
	class AMyBotCharacter_3* BossPawn;
	class APlayerCharacter* PlayerCharacter;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
	class UBoxComponent* WeaponCollision;

};
