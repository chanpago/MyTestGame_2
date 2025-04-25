// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BossEntranceTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API ABossEntranceTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Constructor
	ABossEntranceTriggerBox();

	// Overlap event when player enters the trigger box
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:
	// Sound to play when the player enters the trigger box
	UPROPERTY(EditAnywhere, Category = "Boss Entrance")
	USoundBase* BossEntranceMusic;

	// Function to handle boss entrance logic
	void PlayBossEntranceMusic();
};
