// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossTriggetActor.generated.h"

UCLASS()
class MYTESTGAME_2_API ABossTriggetActor : public AActor
{
	GENERATED_BODY()
	
public:	

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	// Sound to play when player enters the trigger box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Entrance")
	USoundBase* BossEntranceMusic;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	ABossTriggetActor();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DisplayBossUI();


private:

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	
	// Flag to track whether the music has been played
	bool bHasPlayedMusic;

	// Ÿ�̸� �ڵ� ����
	FTimerHandle TimerHandle;

	//=========================================================================
	// FUCTIONS
	//=========================================================================

protected:
	class AMainHUD* MainHUD; // AMainHUD�� ���� ����


	virtual void BeginPlay() override;
};
