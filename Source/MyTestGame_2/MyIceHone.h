#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyIceHone.generated.h"

class UBoxComponent;
class APlayerCharacter;
class AMyTestCharacter_2;

UCLASS()
class MYTESTGAME_2_API AMyIceHone : public AActor
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	AMyIceHone();

	FORCEINLINE class UBoxComponent* GetProjectileCollision() const { return CollisionComp; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void PostInitializeComponents() override;

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	class UBoxComponent* CollisionComp;

	AMyTestCharacter_2* MyPawn;

protected:
	// 게임 시작 또는 스폰될 때 호출됨
	virtual void BeginPlay() override;

};