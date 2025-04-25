// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "BossSkill2_DA.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API ABossSkill2_DA : public ADecalActor
{
	GENERATED_BODY()
	

public:

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	ABossSkill2_DA();

	// Function to initialize the decal with a specific duration
	void InitializeDecal(FVector const& Location, float Duration);


private:

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	FTimerHandle TimerHandle_Destroy;

	//=========================================================================
	// FUCTIONS
	//=========================================================================

	// Function to destroy the decal after the duration
	void DestroyDecal();
};
