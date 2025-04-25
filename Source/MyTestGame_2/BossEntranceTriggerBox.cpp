// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEntranceTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h" 


ABossEntranceTriggerBox::ABossEntranceTriggerBox()
{
	// Register the overlap event
	OnActorBeginOverlap.AddDynamic(this, &ABossEntranceTriggerBox::OnOverlapBegin);
}

void ABossEntranceTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void ABossEntranceTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, " OnOverlapBegin!");
	// Check if the overlapping actor is the player
	if (OtherActor && OtherActor != this)
	{

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OtherActor && OtherActor != this");
		// Cast the overlapping actor to the player character class
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Boss Entrance!");
			// Play the boss entrance music	
			PlayBossEntranceMusic();
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "not OnOverlapBegin!");
	}
}

void ABossEntranceTriggerBox::PlayBossEntranceMusic()
{
	// Check if the sound is valid before playing
	if (BossEntranceMusic)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BossEntranceMusic);
	}
}