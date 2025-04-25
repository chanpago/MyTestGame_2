// Fill out your copyright notice in the Description page of Project Settings.


//game
#include "BossSkill2_DA.h"

//engine
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/DecalComponent.h"


ABossSkill2_DA::ABossSkill2_DA()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set the decal material (make sure the material is set in the editor or dynamically in the game)
	if (GetDecal())
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterial(TEXT("Material'/Game/_My/Decal/DecalCircle.DecalCircle'"));
		if (DecalMaterial.Succeeded())
		{
			GetDecal()->SetDecalMaterial(DecalMaterial.Object);
		}
	}

	// Optional: Set decal size
	GetDecal()->DecalSize = FVector(100.0f, 100.0f, 100.0f);
}

void ABossSkill2_DA::InitializeDecal(FVector const& Location, float Duration)
{
	// Set the location of the decal
	SetActorLocation(Location);

	// Schedule the destruction of the decal after the specified duration
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Destroy, this, &ABossSkill2_DA::DestroyDecal, Duration, false);
}

void ABossSkill2_DA::DestroyDecal()
{
	// Destroy the decal actor
	Destroy();
}